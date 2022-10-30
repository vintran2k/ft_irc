#include "Channel.hpp"

Channel::Channel(std::string const & name, User * admin) :
    _name(name),
    _admin(admin),
    _inviteOnly(false)
{
    _users.insert(admin);
}

Channel::~Channel() {}

bool	Channel::addUser(User * user, std::string const key) {  // in progress

    (void)  key;
    if (_users.find(user) != _users.end())
        return false;
    return true;
}
