#ifndef CONTACT_LISTENER_HPP
#define CONTACT_LISTENER_HPP

#include <box2d/box2d.h>

class Gameplay;
class ContactListener : public b2ContactListener {
public:
    explicit ContactListener(Gameplay* gameplay);
    void BeginContact(b2Contact* contact) override;
private:
    Gameplay* m_Gameplay;
};
#endif