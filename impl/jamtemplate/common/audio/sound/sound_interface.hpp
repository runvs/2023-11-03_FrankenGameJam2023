#ifndef JAMTEMPLATE_SOUNDINTERFACE_HPP
#define JAMTEMPLATE_SOUNDINTERFACE_HPP

#include <audio/sound_groups/group_volume_getter_interface.hpp>

namespace jt {

class SoundInterface {
public:
    /// Update the sound
    virtual void update() = 0;

    /// Get isPlaying
    /// \return true if the sound is playing, false otherwise
    virtual bool isPlaying() const = 0;

    /// Play the sound
    virtual void play() = 0;
    /// Stop the sound (consecutive play will start from the beginning)
    virtual void stop() = 0;
    /// Pause the sound (consecutive play will continue where the sound was stopped)
    virtual void pause() = 0;

    /// Get Volume
    /// This is the currently stored volume that was set. It does not take SoundGroups into account.
    /// \return volume in range 0.0 to 1.0
    virtual float getVolume() const = 0;

    /// Get final Volume
    /// This is the final volume of the sound, based on the volume, the SoundGroup volume and
    /// other factors (e.g. blend).
    /// \return the final Volume in range 0.0 to 1.0
    virtual float getFinalVolume() const = 0;

    /// Set Volume
    /// \param newVolume in range 0.0 to 1.0
    virtual void setVolume(float newVolume) = 0;

    virtual void setVolumeProvider(jt::GroupVolumeGetterInterface& provider) = 0;

    /// Set Pitch
    /// \param pitch new pitch value in range 0 to inf
    virtual void setPitch(float pitch) = 0;
    /// Get pitch
    /// \return pitch value in range 0 to inf
    virtual float getPitch() const = 0;

    /// Set Loop
    /// \param doLoop true if sound should loop, false otherwise
    virtual void setLoop(bool doLoop) = 0;
    /// Get Loop
    /// \return true if sound is looping, false otherwise
    virtual bool getLoop(void) = 0;

    /// Get Sound duration
    /// \return duration in seconds
    virtual float getDuration() const = 0;

    /// Get Position
    /// \return the current playing position in seconds. Note: This is only accurate to around 0.3s
    /// as OpenALpp does not provide realtime audio manipulation.
    virtual float getPosition() const = 0;

    /// Set blend. E.g. used for Effect Sounds. Normal sounds just fade.
    /// \param blend blend parameter (between 0 and 1).
    virtual void setBlend(float blend) = 0;

    /// Get blend
    /// \return blend Parameter (between 0 and 1)
    virtual float getBlend() const = 0;

    /// Get sample rate
    /// \return the sample rate in Hz
    virtual int getSampleRate() const = 0;

    /// Set volume group. A Sound can be part of one volume group. A Sound is always in the "master"
    /// volume group.
    /// \param volumeGroupName name to identify the volume group
    virtual void setVolumeGroup(std::string const& volumeGroupName) = 0;

    virtual ~SoundInterface() = default;

    // no copy, no move
    SoundInterface(SoundInterface const&) = delete;
    SoundInterface(SoundInterface&&) = delete;
    SoundInterface& operator=(SoundInterface const&) = delete;
    SoundInterface& operator=(SoundInterface&&) = delete;

protected:
    // default constructor can only be called from derived classes
    SoundInterface() = default;
};
} // namespace jt

#endif // JAMTEMPLATE_SOUNDINTERFACE_HPP
