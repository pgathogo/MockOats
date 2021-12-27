#include <QDebug>
#include "subjectobserver.h"

Observer::~Observer()
{

}

Observer::Observer()
{

}

Subject::~Subject()
{

}

void Subject::attach(Observer* ob)
{
    m_observers.push_back(ob);
}

void Subject::detach(Observer *)
{

}

void Subject::notify()
{
    for (auto o : m_observers){
        o->update(this);
    }
}

void Subject::notify(Subject* changed_subject)
{
    for(auto o : m_observers){
        o->update(changed_subject);
    }
}


Subject::Subject()
{

}


