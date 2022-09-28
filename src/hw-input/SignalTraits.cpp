#include "SignalTraits.hpp"

using namespace HWInput;

//------------------------------------------------------------------------------
SignalTraits::SignalTraits(const std::string &button, Button control)
: signal("S_" + button + "_BTN")
, reply("R_" + button + "_BTN")
, control(control)
{

}

