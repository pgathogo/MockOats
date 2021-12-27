#pragma once

#include <string>
#include "subjectobserver.h"

namespace OATS{

    class Audio{
    public:
        Audio();
        std::string title();
        std::string artist();
        int duration();

        void set_title(std::string);
        void set_artist(std::string);
        void set_duration(int);
    private:
        std::string m_title;
        std::string m_artist;
        int m_duration;
    };

    class ScheduleItem : public Subject
    {
    public:
        enum SchedType{SONG, COMM, JINGLE, DROP};
        enum ItemStatus{WAITING, CUED, PLAYING, PLAYED};

        ScheduleItem();

        int schedule_ref();
        int index();
        void set_index(int);

        ItemStatus item_status();
        std::string play_channel();
        SchedType schedule_type();
        int hour();
        std::string time();
        Audio audio();

        void set_schedule_ref(int);
        void set_item_status(ItemStatus);
        void set_play_channel(std::string);
        void set_schedule_type(SchedType);
        void set_hour(int);
        void set_time(std::string);
        void set_audio(Audio);

        std::string item_status_text();

    private:
        int m_schedule_ref;
        int m_index;
        ItemStatus m_item_status;
        std::string m_play_channel;
        SchedType m_schedule_type;
        int m_hour;
        std::string m_time;
        Audio m_audio;

    };
}

