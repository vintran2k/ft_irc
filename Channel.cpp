#include "Channel.hpp"

Channel::Channel(std::string const & name, User * admin) :
    _name(name),
    _admin(admin),
    _inviteOnly(false)
{}

Channel::~Channel() {}

