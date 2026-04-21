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
    ~PlaybackController();
    void setAnimationManager(AnimationManager* source);

    bool processInput(RawInputEvent nextInput);
    void update(std::string DSTarget);
    void render();
private:
    AnimationManager* animationManager = nullptr;

    // Initialization 
    Texture playbackMap = {0};
    std::map <PlaybackIcon, Vector2> playbackMapPosition;
    const Vector2 MAP_OFFSET = {117, 69};
    const Vector2 MAP_HORIZONTAL_OFFSET = {190.5, 0}, MAP_VERTICAL_OFFSET = {0, 142.5};
    const float length = 94, scale = 0.5f;

    const float NAVBAR_WIDTH = 95.0f;
    const Vector2 SCREEN_VERTICAL_OFFSET = {0.0f, 100.0f};
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

    static constexpr float SPEED_PRESETS[] = {0.25f, 0.5f, 1.0f, 2.0f, 4.0f};
    static constexpr int NUM_PRESETS = 5;
    int currentSpeedIndex = 2; 

    Vector2 speedButtonPosition = {0.0f, 0.0f};
    void loadSpeedButton();
    bool touchSpeedHitbox(Vector2 queryPosition);
    void renderSpeedLabel();
};
#endif