//g++ music.cpp -o music -lfmt -lwinmm -s
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "music.h"

#define IS_NOTE(c) ((c)>='0'&&(c)<'7')
#define BUFFERSIZE 8*1024

music::music(const char *filename)
{
	FILE *fp = fopen(filename, "r");
	file.raw = new char[BUFFERSIZE]{0};
	fread(file.raw, sizeof(char), BUFFERSIZE, fp);
	fclose(fp);
	
	midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL);
}

bool music::is_identifier(char c) {
        if (c == '+' || c == '-' || c == '#' || 
            c == '.' || c == ',' || IS_NOTE(c)
        )   return true;
        return false;
    }

char music::getch()   //不可能返回空字符，使用的时候注意一下逗号就行了
{
	char c = file.raw[file.offset++];
	if (c=='$')
		return 0;
	if (is_identifier(c))
		return c;
	if (c==' '||c=='\t')
		return getch();
	if (c=='\n') {
		file.line++;
		return getch();
	}
	if (c=='@') {  //注释：返回字符为下一行第一个有效字符
		while (c!='\n')
			c = file.raw[file.offset++];
		c = getch();
		return c;
	}
	if (c=='S') {
		double n = atof(file.raw + file.offset);
		if (n <= 0.0) {
			std::printf("%s: %d: error! %d: %d: %c", __FILE__, __LINE__, file.offset, file.line, c);
			exit(EXIT_FAILURE);
		}
		dft.sleep = 60.0 / n * 1e3;
		file.offset++;
		while ((c>='0'&&c<='9')||c=='.')
			c = file.raw[file.offset++];
		while (c!=',')
			c = file.raw[file.offset++];
		return getch();
	}
	if (c=='V') {
		int n = atoi(file.raw + file.offset);
		if (n == 0) {
			std::printf("%s: %d: error! %d: %d: %c", __FILE__, __LINE__, file.offset, file.line, c);
			exit(EXIT_FAILURE);
		}
		dft.volume = n;
		file.offset++;
		while (c>='0'&&c<='9')
			c = file.raw[file.offset++];
		while (c!=',')
			c = file.raw[file.offset++];
		return getch();
	}
	if (c=='T') {
		int n = atoi(file.raw + file.offset);
		if (n == 0) {
			std::printf("%s: %d: error! %d: %d: %c", __FILE__, __LINE__, file.offset, file.line, c);
			exit(EXIT_FAILURE);
		}
		midiOutShortMsg(handle, n<<8 | dft.channel);
        //play_note({1,{n,0,}})
		file.offset++;
		while (c>='0'&&c<='9')
			c = file.raw[file.offset++];
		while (c!=',')
			c = file.raw[file.offset++];
		return getch();
	}
	return c;
}

/*
  blank={ ...}{\t...},{ ...}{\n...}
  一共有这几种语句：
  Speed: S{unsigned int}$blank
  Timbre: T{byte}$blank
  Volume: V{byte}$blank

  {+-}[0..7]#.{/...}{-...}$blank
*/

Notes music::get_note()
{
	char c = getch();
	if (c==0)
		return {0, nullptr};

	//返回值结构体
	Notes ret{
		1,
		new Note {
			dft.timbre,    //默认音色为钢琴
			dft.volume,
			48,    //默认中央C
			9<<4,  //默认命令为播放
    		dft.channel,
    		dft.sleep
		}
	};

	int segment = 4, offset = 0;
	//  段/8度       音符/偏移量

	//确定段
	if (c == '+' || c == '-') {
		while (c == '+') {
			segment++;
			c = getch();
		}
		while (c == '-') {
			segment--;
			c = getch();
		}
	}

	if (!IS_NOTE(c)) {
		fmt::print("{}: {}: error! {}: {}: {}", __FILE__, __LINE__, file.offset, file.line, c);
		exit(EXIT_FAILURE);
	}

	//空格音不用计算、没有升半音
	if (c == '0')
		ret.notes->scale = 128;
	else {
		offset = c - 48 - 1;   //数字化
		c = getch();

		const int arr[] {0, 2, 4, 5, 7, 9, 11};
		offset = arr[offset];  //散列化、标准化

		//确定升半音
		if (c == '#') {
			if (offset%12==4||offset%12==11) {
				fmt::print("{}: {}: error! {}: {}: {}", __FILE__, __LINE__, file.offset, file.line, c);
				exit(EXIT_FAILURE);
			}
			offset++;
			c = getch();
		}

		ret.notes->scale = segment * 12 + offset;
	}

	//确定音符长度
	if (c=='.') {
		ret.notes->sleep *= 1.5;
		c = getch();
	} else
		while (c=='/') {
			ret.notes->sleep /= 2;
			c = getch();
		}
	if (c=='-') {
		int n = 0;
		while (c=='-') {
			n++;
			c = getch();
		}
		ret.notes->sleep += n * dft.sleep;
	}
	while (c!=','&&c!='$')
			c = file.raw[file.offset++];
	if(c=='$')
		return {0, nullptr};
	return ret;
}

bool music::play_note(Notes arg) {
    if(arg.num_note == 0 || arg.notes == nullptr)
        return false;
    midiOutShortMsg(
        handle, 
        arg.notes->volume<<16  |
        arg.notes->scale<<8    |
        arg.notes->cmd         |
        arg.notes->channel
    );
    Sleep(arg.notes->sleep);
    return true;
}

void music::play() {
    while(play_note(get_note()));
}
