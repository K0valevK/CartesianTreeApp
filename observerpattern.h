#ifndef GRAPHICPROJECT_OBSERVERPATTERN_H
#define GRAPHICPROJECT_OBSERVERPATTERN_H

namespace GraphicProject {

class Observer {
public:
    virtual ~Observer(){}
    virtual void Notify() = 0;
};

class Observable {
public:
    void Subscribe(Observer* observer);
    void Unsubscribe();

    void Notify();

protected:
    Observer *observer_;
};

} // namespace GraphicProject

#endif // GRAPHICPROJECT_OBSERVERPATTERN_H
