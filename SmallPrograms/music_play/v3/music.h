#ifndef MUSIC_H
#define MUSIC_H

#include <Windows.h>
#include <fmt/core.h>

struct Note {
    byte timbre;   //音色
    byte volume;   //音量
    byte scale;    //音阶
    byte cmd;      //命令
    byte channel;  //通道
    int sleep;     //长度
};

struct Notes {
    size_t num_note;
    Note *notes;
};

class music {
private:
    struct {
        char *raw = nullptr;
        int offset = 0;
        int line = 0;
    } file;        
    struct {
        byte volume = 0x7f;
        byte timbre = 0;   //默认音色为钢琴
        byte channel = 4;
        int sleep = 500;
    } dft;
    HMIDIOUT handle;
    
    bool is_identifier(char c);
    bool play_note(Notes);
    Notes get_note();
    char getch();
    void putch() {
        file.offset--;
    }
public:
    music() { };
    music(const char *);
    ~ music() {
        delete []file.raw;
        midiOutClose(handle);
    };
    void print() {
        for(int i=0;file.raw[i]!='$';i++)
            putchar(file.raw[i]);
    }
    void play();
};

#endif
