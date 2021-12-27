#include "scheduleitem.h"

namespace OATS{

    Audio::Audio()
    {
    }

    std::string Audio::title()
    {
        return m_title;
    }

    std::string Audio::artist()
    {
        return m_artist;
    }

    int Audio::duration()
    {
        return m_duration;
    }

    void Audio::set_title(std::string title)
    {
        m_title =  title;
    }

    void Audio::set_artist(std::string art)
    {
        m_artist = art;
    }

    void Audio::set_duration(int dur)
    {
        m_duration = dur;
    }

    ScheduleItem::ScheduleItem()
        :m_schedule_ref{-1}
        ,m_index{-1}
    {
    }

    int ScheduleItem::schedule_ref()
    {
        return m_schedule_ref;
    }

    int ScheduleItem::index()
    {
        return m_index;
    }

    void ScheduleItem::set_index(int i)
    {
        m_index = i;
    }

    ScheduleItem::ItemStatus ScheduleItem::item_status()
    {
        return m_item_status;
    }

    std::string ScheduleItem::play_channel()
    {
        return m_play_channel;
    }

    ScheduleItem::SchedType ScheduleItem::schedule_type()
    {
        return m_schedule_type;
    }

    int ScheduleItem::hour()
    {
        return m_hour;
    }
    std::string ScheduleItem::time()
    {
        return m_time;
    }
    Audio ScheduleItem::audio()
    {
        return m_audio;
    }

    void ScheduleItem::set_schedule_ref(int sched_ref)
    {
        m_schedule_ref = sched_ref;
    }
    void ScheduleItem::set_item_status(ScheduleItem::ItemStatus item_status)
    {
        m_item_status = item_status;
    }
    void ScheduleItem::set_play_channel(std::string channel)
    {
        m_play_channel = channel;
    }
    void ScheduleItem::set_schedule_type(ScheduleItem::SchedType sched_type)
    {
        m_schedule_type = sched_type;
    }
    void ScheduleItem::set_hour(int hr)
    {
        m_hour = hr;
    }
    void ScheduleItem::set_time(std::string time)
    {
        m_time = time;
    }
    void ScheduleItem::set_audio(Audio audio)
    {
        m_audio = audio;
    }

    std::string ScheduleItem::item_status_text()
    {
        switch (item_status())
        {
        case WAITING:
            return "WAITING";
            break;
        case CUED:
            return "CUED";
            break;
        case PLAYING:
            return "PLAYING";
            break;
        default:
            return "WAITING";
        }
    }

}
