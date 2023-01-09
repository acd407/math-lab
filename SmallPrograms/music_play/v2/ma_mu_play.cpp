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
            c == '+' || c == '-' || c == '@' || \
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
    
    if(!is_identifier(c))
        return false;
    switch (c) {
        case '@':   //注释：一直读到行尾，下一字符为下一行第一个
            while (getch()!='\n');
        case ' ' :
        case '\n':
        case '\t':
            return true; //空字符：跳过
        case 'L':
            note.sleep = HIGH_SPEED;
        case 'M':
            note.sleep += SPEED_INTERVAL;
        case 'H':
            note.sleep += SPEED_INTERVAL;
    }
    //控制字符
    if (c == 'L' || c == 'H' || c == 'M') {
        if (c == 'L')
            note.sleep = LOW_SPEED;
        else if (c == 'H')
            note.sleep = HIGH_SPEED;
        else if (c == 'M')
            note.sleep = MIDDLE_SPEED;
        c = getch();
        return true;
    }
    
    int note_segment = 1;   //8度
    int note_offset = 0;    //偏移
    int note_sleep = note.sleep;//Custom持续时间
    
    if(c == '+' || c == '-') {  //8度
        if (c == '+')
            note_segment = 2;
        else if (c == '-')
            note_segment = 0;
        c = getch();
    }
    
    if(!is_note(c)) {
        fmt::print("{}: {}: error! {}: {}", __FILE__, __LINE__, offset, c);
        exit(0);
    }
    note_offset = c-48-1;   //偏移
    c = getch();
    
    if(c=='.') {
        note_sleep *= 1.5;
        c = getch();
    } else if(c=='-') {
        note_sleep *= 1.5;
        c = getch();
    } else
        while (c=='/') {
            note_sleep /= 2;
            c = getch();
        }
    
    if (c!=',') {
        fmt::print("{}: {}: error! {}: {}", __FILE__, __LINE__, offset, c);
        exit(0);
    }
    
    int note_table[3][7] = {
        {C3, D3, E3, F3, G3, A3, B3},
        {C4, D4, E4, F4, G4, A4, B4},
        {C5, D5, E5, F5, G5, A5, B5},
    };
    
    if(note_offset>=0) {
        int note_voice = (note.volume << 16) + \
            (note_table[note_segment][note_offset] << 8) + 0x94;
        char tr[3] = {'L', 'M', 'H'};
        fmt::print("{:c}{:c}\n", tr[note_segment], '1'+note_offset);
        midiOutShortMsg(handle, note_voice);
    }
    Sleep(note_sleep);
    return true;
}

int main () {
    music mu("mu.txt");
    mu.play();
    return 0;
}
