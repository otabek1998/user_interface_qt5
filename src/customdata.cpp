#include "include/customdata.h"

void CustomData::setArtist(std::string str)
{
    this->artist = str;
    emit this->onArtistChange();
}

void CustomData::setSongName(std::string str)
{
    this->song_name = str;
    emit this->onSongNameChange();
}
