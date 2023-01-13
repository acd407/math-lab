//g++ ma_mu_play.cc -o ma_mu_play -lfmt -lwinmm -s
#include <fstream>
#include <fmt/core.h>
#include <Windows.h>
#include <map>
#include "ma_mu.h"

class music {
private:
    char *string = nullptr;
    int offset = 0;
    int line = 0;
    long size = 0;
    struct {
        int volume = 0x7f;
        int sleep = 300;
    } note;
    HMIDIOUT handle;
    bool is_note(char c) {
        if(c>=48&&c<56)
            return true;
        return false;
    }
    bool is_identifier(char c) {
        if( c == 'L' || c == 'H' || c == 'M' || \
            c == '+' || c == '-' || c == '#' || \
            c == ' ' || c == '\n'|| c == '\t'|| \
            is_note(c)
        )   return true;
        return false;
    }
    char getch() {
        return string[offset++];
    }
    void putch() {
        offset--;
    }
    bool play_note();
public:
    music() { };
    music(const char *);
    ~ music() {
        delete []string;
        midiOutClose(handle);
    };
    void print() {
        fmt::print(string);
    }
    void play() {
        while (play_note());
    }
};

music::music(const char *filename) {
    std::ifstream filestr;
    filestr.open (filename);
    std::filebuf *pbuf=filestr.rdbuf();
    size = pbuf->pubseekoff (0,std::ios::end,std::ios::in);
    pbuf->pubseekpos (0,std::ios::in);
    string=new char[size];
    pbuf->sgetn (string,size);
    filestr.close();
    
    midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL);
}

bool music::play_note() {
    char c = getch();
    //注释：一直读到行尾，下一字符为下一行第一个
    if(c=='@') {
        while (getch()!='\n');
        line++;
        return true;
    }
    if(!is_identifier(c))
        return false;
    //空字符：跳过
    if(c==' '||c=='\t')
        return true;
    if(c=='\n') {
        line++;
        return true;
    }
    
    //控制字符
    if (c == 'L' || c == 'H' || c == 'M') {
        if (c == 'L')
            note.sleep = LOW_SPEED;
        else if (c == 'H')
            note.sleep = HIGH_SPEED;
        else
            note.sleep = MIDDLE_SPEED;
        while (c != ',')
            c = getch();
        return true;
    }
    
    //----流程开始----
    //'  +#5/// ,'
    int note_segment = 1;   //初始8度
    int note_offset = 0;    //偏移
    int note_sleep = note.sleep;//Custom持续时间
    bool up_half_note = false;  //升半音
    
    //确定8度
    if(c == '+' || c == '-') {
        if (c == '+')
            note_segment = 2;
        else if (c == '-')
            note_segment = 0;
        c = getch();
    }
    
    //确定升半音
    if(c == '#') {
        up_half_note = true;
        c = getch();
    }
    
    if(!is_note(c)) {
        fmt::print("{}: {}: error! {}: {}: {}", __FILE__, __LINE__, offset, line, c);
        exit(0);
    }
    note_offset = c-48-1;   //偏移
    c = getch();
    
    //音符长度控制
    if(c=='.') {
        note_sleep *= 1.5;
        c = getch();
    } else
        while (c=='/') {
            note_sleep /= 2;
            c = getch();
        }
        
    if(c=='-') {
        int n = 0;
        while (c=='-') {
            n++;
            c = getch();
        }
        note_sleep += n * note.sleep;
    }
    
    while (c == ' '||c == '\t')
        c = getch();
    if (c!=',') {
        fmt::print("{}: {}: error! {}: {}: {}", __FILE__, __LINE__, offset, line, c);
        exit(0);
    }
    
    int note_table[3][7] = {
        {C3, D3, E3, F3, G3, A3, B3},
        {C4, D4, E4, F4, G4, A4, B4},
        {C5, D5, E5, F5, G5, A5, B5},
    };
    
    //等于0，则跳过
    if(note_offset>=0) {
        int note_value = note_table[note_segment][note_offset];
        if (up_half_note)
            ++note_value;
        int note_voice = (note.volume << 16) + (note_value << 8) + 0x94;
        //输出音调
        char tr[3] = {'L', 'M', 'H'};
        fmt::print("{:c}{:c}{:c}  {}\n", tr[note_segment], '1'+note_offset, up_half_note?'s':' ', note_sleep);
        midiOutShortMsg(handle, note_voice);
    }
    Sleep(note_sleep);
    return true;
}

void piano() {
	HMIDIOUT handle;
	midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL);
	std::map<char, int>v = {
		{'Z',C3},{'X',D3},{'C',E3},{'V',F3},{'B',G3},{'N',A3},{'M',B3},
		{'A',C4},{'S',D4},{'D',E4},{'F',F4},{'G',G4},{'H',A4},{'J',B4},
		{'Q',C5},{'W',D5},{'E',E5},{'R',F5},{'T',G5},{'Y',A5},{'U',B5},
	};
	while (1) {
		for (char i = 'A'; i <= 'Z'; i++) {
			if (GetKeyState(i) < 0) {
				midiOutShortMsg(handle, (0x007f << 16) + (v[i] << 8) + 0x90);
				while (GetKeyState(i) < 0)Sleep(100);
			}
		}
	}
}


int main (int argc, char** argv) {
    if(argc==2) {
        music mu(argv[1]);
        mu.play();
    } else
        piano();
    return 0;
}
