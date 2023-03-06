#pragma once

#include <Windows.h>
#include <fmt/core.h>
#include <interupt.h>

struct Note {
    byte timbre;   //音色
    byte volume;   //音量
    byte scale;    //音阶
    byte cmd;      //命令
    byte channel;  //通道
    int  sleep;    //长度
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
    int default_segment, default_offset;
    HMIDIOUT handle;
    Note default_Note;
    bool is_identifier(char c);
    bool play_note(Notes);
    Notes get_note();
    char getunblank();
    char getch();
    void putch();
public:
    music(const char *);
    ~music();
    void print();
    void play();
};

