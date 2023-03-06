#include <music.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

template<typename T>
constexpr auto IS_NOTE(T c) { return ((c) >= '0' && (c) < '7'); }
#define BUFFERSIZE 8 * 1024

music::music(const char *filename) {
    FILE *fp;
    fopen_s(&fp, filename, "r");
    if (fp == nullptr)
        ERR("file open error");
    file.raw = new char[BUFFERSIZE] {0};
    if (file.raw == nullptr)
        ERR("buffer allocate error");
    fread(file.raw, sizeof(char), BUFFERSIZE, fp);
    fclose(fp);

    // 默认音符
    default_Note = *new Note {
        0,    // 默认音色为钢琴
        0x7f,
        48,    // 默认中央C
        9 << 4,  // 默认命令为播放
        4,
        500
    };
    
    // 默认 scale 段和偏移量
    default_segment = 4;
    default_offset = 0;
    
    midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL);
}

music::~music() {
    delete[]file.raw;
    midiOutClose(handle);
};

// 一个Note中可能出现的第一个 identifier
bool music::is_identifier(char c) {
    if (c == '+' || c == '-' || IS_NOTE(c))
        return true;
    return false;
}

// 返回非空字符
char music::getunblank() {
    char c = file.raw[file.offset++];
    while (c == ' ' || c == '\t' || c == '\n') {
        if (c == '\n')
            file.line++;
        c = file.raw[file.offset++];
    }
    return c;
}

// 返回的只是Note，CMD在函数内被处理
// 一个完整的Note，只返回第一个字符
char music::getch() // 不帮忙清逗号，因为多字符没法弄
{
    char c = getunblank();
    if (c == '$')   // 结束分支
        return 0;
    if (is_identifier(c))   // 标准字符分支
        return c;
    if (c == '@') { // 注释：返回字符为下一行第一个有效字符
        while (c != '\n')
            c = file.raw[file.offset++];
        c = getch();
        return c;
    }
    // 功能处理分支
    if (c == 'S') { // Sleep
        double n = atof(file.raw + file.offset);
        if (n <= 0.0) {
            WAR("illegal sleep input, set sleep to default");
            n = default_Note.sleep;
        }
        default_Note.sleep = int(60.0 / n * 1e3);
        file.offset++;
        while ((c >= '0' && c <= '9') || c == '.')
            c = file.raw[file.offset++];
        while (c != ',')
            c = file.raw[file.offset++];
        return getch();
    }
    if (c == 'V') { // Volume
        int n = atoi(file.raw + file.offset);
        if (n == 0) {
            WAR("illegal volume input, set volume to default");
            n = default_Note.volume;
        }
        default_Note.volume = n;
        file.offset++;
        while (c >= '0' && c <= '9')
            c = file.raw[file.offset++];
        while (c != ',')
            c = file.raw[file.offset++];
        return getch();
    }
    if (c == 'T') { // Timbre
        int n = atoi(file.raw + file.offset);
        if (n == 0) {
            WAR("illegal timbre input, set timbre to default");
            n = default_Note.timbre;
        }
        midiOutShortMsg(handle, n << 8 | default_Note.channel);
        // play_note({1,{n,0,}})
        file.offset++;
        while (c >= '0' && c <= '9')
            c = file.raw[file.offset++];
        while (c != ',')
            c = file.raw[file.offset++];
        return getch();
    }
    if (c == 'O') { // Octave
		int segment = default_segment;
		c = getch();
		switch (c) {
			case '+':
				while (c == '+') {
					segment++;
					c = getch();
				} break;
			case '-':
				while (c == '-') {
					segment--;
					c = getch();
				} break;
			default:
				WAR("segment number not set, set to default");
		}
		default_segment = segment;
        if (c != ',') {
            WAR(fmt::format("expect ',', but acturlly get: {:c}, skipping", c));
            while (c != ',')
				c = file.raw[file.offset++];
        }
        return getch();
    }
    return c;
}

/*
  blank={ ...}{\t...},{ ...}{\n...}
  一共有这几种语句：
  Speed:  S{unsigned int}$blank
  Timbre: T{byte}$blank
  Volume: V{byte}$blank

  {+-}[0..7]#.{/...}{-...}$blank
*/

Notes music::get_note() {
    char c = getch();
    if (c == 0)
        return {0, nullptr};

    // 返回值结构体
    Notes ret {
        1,
        new Note(default_Note)
    };

    int segment = default_segment, offset = default_offset;
    //  段/8度                      音符/偏移量

    // 确定段
    switch (c) {
        case '+':
            while (c == '+') {
                segment++;
                c = getch();
            } break;
        case '-':
            while (c == '-') {
                segment--;
                c = getch();
            } break;
    }

    if (!IS_NOTE(c))
        ERR(fmt::format("next char is not note, is {}",c));

    // 空格音不用计算、没有升半音
    if (c == '0') {
        ret.notes->scale = 128;
        c = getch();
    }
    else {
        offset = c - 48 - 1; // 数字化
        c = getch();

        // C, C#, D, D#, E, F, F#, G, G#, A, A#, B
        // 0  1   2  3   4  5  6   7  8   9  10  11
        const int arr[] {0, 2, 4, 5, 7, 9, 11};
        offset = arr[offset]; // 散列化、标准化

        // 确定升半音
        if (c == '#') {
            if (offset % 12 == 4 || offset % 12 == 11) {
                WAR(fmt::format("{:c} do not have a semitone, ignoring '#'", c));
            } else
                offset++;
            c = getch();
        }

        ret.notes->scale = segment * 12 + offset;
    }
    
    // 3.- 等价于 3/--
    // 确定音符长度
    if (c == '.') { // 延长一半
        ret.notes->sleep += ret.notes->sleep / 2;
        c = getch();
    } else
        while (c == '/') { // 每一个 '/' 缩短一半
            ret.notes->sleep /= 2;
            c = getch();
        }
    
    if (c == '-') { // 延长 n 个 '-'
        int n = 0;
        while (c == '-') {
            n++;
            c = getch();
        }
        ret.notes->sleep += n * default_Note.sleep;
    }
    
    // && c != '$' 直接使用 '$' 是不被允许的
    while (c != ',') {
        if (c != ' ' && c != '\t')
            INF(fmt::format("non-null character skiped, the char is: {:c}",c));
        c = getunblank();
    }
    return ret;
}

bool music::play_note(Notes arg) {
    if (arg.num_note == 0 || arg.notes == nullptr)
        return false;
    midiOutShortMsg(handle, arg.notes->volume << 16 | arg.notes->scale << 8 |
        arg.notes->cmd | arg.notes->channel);
    Sleep(arg.notes->sleep);
    return true;
}

void music::print() {
    for (int i = 0; file.raw[i] != '$'; i++) {
        if (file.raw[i] == '\0') {
            INF("forget to write the end sign '$'");
            return;
        }
        putchar(file.raw[i]);
    }
    putchar('\n');
}

void music::play() {
    while (play_note(get_note()))
        ;
}

void music::putch() {
    file.offset--;
}