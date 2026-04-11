#include <Core/PlaybackController.hpp>
#include <Graphics/Helper.hpp>

PlaybackController::PlaybackController() {
    loadSprite();
}
PlaybackController::~PlaybackController() {
    UnloadTexture(playbackMap);
}

///////////////////////////////
///     GLOBAL FUNCTIONS    ///
///////////////////////////////

void PlaybackController::setAnimationManager(AnimationManager* source) {
    animationManager = source;
}

/// @brief Processing inputs for Playback Controller core 
void PlaybackController::processInput(RawInputEvent nextInput) {
    if (nextInput.inputType != RawInputEvent::InputType::LEFT_MOUSE_CLICKED) return;

    // The "PLAY" button is triggered.
    // If the animation is paused, it will start going forward
    // Otherwise, it is forced to stop at the current snapshot.
    if (touchPlaybackHitbox(playbackScreenPosition[PlaybackIcon::PLAY], nextInput.position)) {
        if (playbackState == PlaybackState::PAUSED) {
            playbackState = PlaybackState::PLAYING;
            animationManager -> setTransitionCoeff(+1);
            animationManager -> resetAnimationTimer();
        } else {
            animationManager -> setTransitionCoeff(0);
            playbackState = PlaybackState::PAUSED;
        }
    }

    animationManager -> setPauseToggle(playbackState == PlaybackState::PAUSED);
    if (playbackState == PlaybackState::PLAYING) return;
    if (animationManager -> getTransitionCoeff() != 0) return;

    // SKIP_START is triggered => Return back to the empty state
    if (touchPlaybackHitbox(playbackScreenPosition[PlaybackIcon::SKIP_START], nextInput.position)) {
        animationManager -> setSnapshotIdx(DSOption, 0);
    }
    
    // If we want to go backward, the database is required to have a previous snapshot.
    // Then, We set the animation to go backward.
    // Note that the playback control has to be in PAUSED state currently.
    if (touchPlaybackHitbox(playbackScreenPosition[PlaybackIcon::STEP_BACKWARD], nextInput.position) && isGoingBackwardValid()) {
        animationManager -> setTransitionCoeff(-1);
        animationManager -> resetAnimationTimer();
    }

    // Similar case also with going forward.
    if (touchPlaybackHitbox(playbackScreenPosition[PlaybackIcon::STEP_FORWARD], nextInput.position) && isGoingForwardValid()) {
        animationManager -> setTransitionCoeff(+1);
        animationManager -> resetAnimationTimer();
    }

    // SKIP_END is triggered => Fast forward to the current snapshot
    if (touchPlaybackHitbox(playbackScreenPosition[PlaybackIcon::SKIP_END], nextInput.position)) {
        animationManager -> setSnapshotIdx(DSOption, animationManager -> getSize(DSOption) - 1);
    }
}

/// @brief Update Data Structure Option 
void PlaybackController::update(std::string DSTarget) {
    DSOption = DSTarget;
}

/// @brief Control button rendering
void PlaybackController::render() {
    playbackRender(PlaybackIcon::SKIP_START, playbackState == PlaybackState::PLAYING);
    playbackRender(PlaybackIcon::STEP_BACKWARD, !isGoingBackwardValid());
    
    if (playbackState == PlaybackState::PLAYING) {
        playbackRender(PlaybackIcon::PAUSE);
    } else {
        playbackRender(PlaybackIcon::PLAY);
    }

    playbackRender(PlaybackIcon::STEP_FORWARD, !isGoingForwardValid());
    playbackRender(PlaybackIcon::SKIP_END, playbackState == PlaybackState::PLAYING);
}

///////////////////////////////////////
///     SUPPORTIVE FUNCTIONS        ///
///////////////////////////////////////

/// @brief Initialization
void PlaybackController::loadSprite() {
    playbackMap = LoadTexture("../Assets/Graphics/playback-map-transparent.png");
    SetTextureFilter(playbackMap, TEXTURE_FILTER_BILINEAR);
    
    playbackMapPosition[PlaybackIcon::PLAY] = MAP_OFFSET;
    playbackMapPosition[PlaybackIcon::PAUSE] = MAP_OFFSET + MAP_HORIZONTAL_OFFSET;
    playbackMapPosition[PlaybackIcon::SKIP_START] = MAP_OFFSET + MAP_HORIZONTAL_OFFSET * 2;
    playbackMapPosition[PlaybackIcon::SKIP_END] = MAP_OFFSET + MAP_HORIZONTAL_OFFSET * 3;
    playbackMapPosition[PlaybackIcon::STEP_BACKWARD] = MAP_OFFSET + MAP_HORIZONTAL_OFFSET * 4;
    playbackMapPosition[PlaybackIcon::STEP_FORWARD] = MAP_OFFSET + MAP_HORIZONTAL_OFFSET * 5;

    const Vector2 SCREEN_OFFSET = Vector2 {(NAVBAR_WIDTH - length * scale) * 0.5f, 300.0f};
    playbackScreenPosition[PlaybackIcon::SKIP_START] = SCREEN_OFFSET;
    playbackScreenPosition[PlaybackIcon::STEP_BACKWARD] = SCREEN_OFFSET + SCREEN_VERTICAL_OFFSET;
    playbackScreenPosition[PlaybackIcon::PLAY] = playbackScreenPosition[PlaybackIcon::PAUSE] = SCREEN_OFFSET + SCREEN_VERTICAL_OFFSET * 2;
    playbackScreenPosition[PlaybackIcon::STEP_FORWARD] = SCREEN_OFFSET + SCREEN_VERTICAL_OFFSET * 3;
    playbackScreenPosition[PlaybackIcon::SKIP_END] = SCREEN_OFFSET + SCREEN_VERTICAL_OFFSET * 4;
}

/// @brief Check button hitboxes with the mouse position 
bool PlaybackController::touchPlaybackHitbox(Vector2 screenPosition, Vector2 queryPosition) {
    Vector2 buttonCenter = screenPosition + Vector2{length, length} * scale * 0.5f;
    return Vector2Distance(buttonCenter, queryPosition) <= length * scale * 0.5f;
}

/// @brief Drawing buttons with customizations
void PlaybackController::playbackRender(PlaybackIcon playbackIcon, bool isDisabled) {
    Vector2 mapPosition = playbackMapPosition[playbackIcon];
    Vector2 screenPosition = playbackScreenPosition[playbackIcon];

    if (isDisabled) {
        DrawTexturePro(
            playbackMap,
            Helper::createRaylibRectangle(mapPosition, mapPosition + Vector2{length, length}), 
            Helper::createRaylibRectangle(screenPosition, screenPosition + Vector2{length, length} * scale),
            {0.0f, 0.0f},
            0.0f,
            GRAY
        );

        return;
    }

    DrawTexturePro(
        playbackMap,
        Helper::createRaylibRectangle(mapPosition, mapPosition + Vector2{length, length}), 
        Helper::createRaylibRectangle(screenPosition, screenPosition + Vector2{length, length} * scale),
        {0.0f, 0.0f},
        0.0f,
        touchPlaybackHitbox(screenPosition, GetMousePosition()) ? BLUE : WHITE
    );
}

bool PlaybackController::isGoingBackwardValid() {
    return animationManager -> getTransitionCoeff() == 0 && animationManager -> canStepBackward(DSOption);
}
bool PlaybackController::isGoingForwardValid() {
    return animationManager -> getTransitionCoeff() == 0 && animationManager -> canStepForward(DSOption);
}

