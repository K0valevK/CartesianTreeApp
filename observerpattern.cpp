#include "observerpattern.h"

namespace GraphicProject {

void Observable::Subscribe(Observer *observer) {
    observer_ = observer;
}

void Observable::Unsubscribe() {
    observer_ = nullptr;
}

void Observable::Notify() {
    observer_->Notify();
}

} // namespace GraphicProject
