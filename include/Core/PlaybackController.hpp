#ifndef PLAYBACKCONTROLLER_HPP
#define PLAYBACKCONTROLLER_HPP

#include <Common.hpp>
#include <Core/AnimationManager.hpp>
#include <Core/InputManager.hpp>

#include <Graphics/UIManager.hpp>

enum class PlaybackState {
    PLAYING,
    PAUSED
};
enum class PlaybackIcon {
    PLAY,
    PAUSE,
    SKIP_START,
    SKIP_END,
    STEP_BACKWARD,
    STEP_FORWARD
};

class PlaybackController {
public:
    PlaybackController();
    void setAnimationManager(AnimationManager* source);

    void processInput(RawInputEvent nextInput);
    void update(std::string DSTarget);
    void render();
private:
    AnimationManager* animationManager = nullptr;

    // Initialization 
    Texture playbackMap;
    std::map <PlaybackIcon, Vector2> playbackMapPosition;
    const Vector2 MAP_OFFSET = {117, 69};
    const Vector2 MAP_HORIZONTAL_OFFSET = {190.5, 0}, MAP_VERTICAL_OFFSET = {0, 142.5};
    const float length = 94, scale = 0.75f;

    const float BUTTON_Y_BOTTOM_OFFSET = 100.0f;
    const Vector2 SCREEN_HORIZONTAL_OFFSET = {100.0f, 0.0f};
    std::map <PlaybackIcon, Vector2> playbackScreenPosition;
    void loadSprite();

    // Data structure that the playback controller is managing
    std::string DSOption = "";

    // Playback state
    PlaybackState playbackState = PlaybackState::PLAYING;
    bool touchPlaybackHitbox(Vector2 screenPosition, Vector2 queryPosition);
    void playbackRender(PlaybackIcon playbackIcon, bool isDisabled = false);
    
    bool isGoingBackwardValid();
    bool isGoingForwardValid();
};
#endif