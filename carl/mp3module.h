// Copyright (c) 2021 Jan Delgado <jdelgado[at]gmx.net>
// https://github.com/jandelgado/carl
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//
#pragma once
#include "log.h"
#include "mp3_driver.h"

/*
 * Frontend to the DFPlayerMini module
 */
class Mp3Module {
 private:
    enum class eEvent { NONE, PLAYPAUSE, STOP, NEXT, PREV, PLAY_SONG };
    // how often to retry to read a folders
    constexpr static auto kMaxFolderReadTries = 3;

 public:
    enum class ePlayMode { REPEAT, PLAY_ONCE, SINGLE_SONG };

    struct SongInfo {
        SongInfo(bool found, uint8_t folder, uint16_t song)
            : found_(found), folder_(folder), song_(song) {}
        bool found_;
        uint8_t folder_;
        uint16_t song_;
    };

    Mp3Module() = delete;

    Mp3Module(Mp3Driver* mp3_driver, ePlayMode skip_mode);

    void setSkipMode(ePlayMode skip_mode) { skip_mode_ = skip_mode; }

    void setEventPlayPause() { event_ = eEvent::PLAYPAUSE; }
    void setEventPlaySong(uint8_t folder, uint16_t song) {
        event_ = eEvent::PLAY_SONG;
        next_song_ = song;
        next_folder_ = folder;
    }
    void setEventStop() { event_ = eEvent::STOP; }
    void setEventPrev() { event_ = eEvent::PREV; }
    void setEventNext() { event_ = eEvent::NEXT; }

    // update players internal state, call periodically from main loop
    void update();

    // set internal folder and song which will be played on next Play event
    void setNextSong(uint8_t folder, uint8_t song);

    void setVolume(uint8_t volume);

    // set the EQ (0..5)
    void setEqMode(uint8_t mode);

    // cycle through eq prests
    void nextEqMode();

    // returns true if the player (hardware) is currently playing, otherwise
    // false.
    bool isBusy() const;

    // randomly get a song from a folder
    SongInfo getRandomSongFromFolder(uint8_t folder) const;

 private:
    // start to play given file in folder. count starts with 1 for file and
    // folder. Updates curFile and cur_folder_ pointers.
    void playSongFromFolder(uint8_t folder, uint16_t song);

    // song navigation
    SongInfo getNextSong() const;
    SongInfo getPrevSong() const;
    SongInfo getCurSong() const {
        return SongInfo(getNumSongs(cur_folder_) > 0, cur_folder_, cur_song_);
    }
    int getNumSongs(uint8_t folder) const { return folder_count_[folder]; }

    // returns true if the player is playing for at least timespan ms
    bool isCurSongPlayingSince(uint32_t kTimespan) const;

 private:
    enum class eState {
        PAUSED,
        STOPPED,
        START_PLAYING,
        WAIT_FOR_PLAYER_TO_START,
        PLAYING,
    };

    eEvent consumeEvent() {
        auto e = event_;
        event_ = eEvent::NONE;
        return e;
    }

    // CONFIG: if PREV is pressed and the current song plays less then
    // kJumpPrevThresh ms, then jump to the previous song, else restart current
    // song.
    static auto constexpr kJumpPrevThresh = 3000;
    static auto constexpr kTimeoutStartPlayingMs = 1000;
    static auto constexpr kTimeWaitPlayerToStart = 1000;

    // CONFIG: index (starting with 0) of first and last folder containing
    // playlists (corresponds to folders 01/ .. 09/ on SD card)
    static auto constexpr kFirstFolder = 0;
    static auto constexpr kLastFolder = 8;

    // max number of supported folders. Carl suports 9 folders which can be
    // directly accessed with the folder buttons plus 1 folder for the startup
    // jingles.
    static auto constexpr kMaxFolders = (kLastFolder - kFirstFolder + 1) + 1;

    Mp3Driver* mp3_driver_;

    eState state_ = eState::STOPPED;
    eEvent event_ = eEvent::NONE;
    ePlayMode skip_mode_;

    unsigned long song_playing_since_ = 0;
    unsigned long time_start_playing_ = 0;

    uint8_t cur_folder_ = 0;  // current folder. Indexing starts with 0.
    uint16_t cur_song_ = 0;   // current song. Indexing starts with 0.

    uint8_t next_folder_ = 0;
    uint8_t next_song_ = 0;

    uint16_t folder_count_[kMaxFolders];
    uint8_t eq_mode_ = 5;
};
