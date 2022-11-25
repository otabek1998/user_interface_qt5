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

void CustomData::setAlbumArt(GstMapInfo map)
{
    //(void)memcpy(this->album_art, map.data, map.size);
    this->album_art = map.data;
    this->album_art_size = map.size;
    emit this->onAlbumArtChange();
}
