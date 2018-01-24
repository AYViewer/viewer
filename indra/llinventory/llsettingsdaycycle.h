/**
* @file llsettingsdaycycle.h
* @author optional
* @brief A base class for asset based settings groups.
*
* $LicenseInfo:2011&license=viewerlgpl$
* Second Life Viewer Source Code
* Copyright (C) 2017, Linden Research, Inc.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation;
* version 2.1 of the License only.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*
* Linden Research, Inc., 945 Battery Street, San Francisco, CA  94111  USA
* $/LicenseInfo$
*/

#ifndef LL_SETTINGS_DAYCYCLE_H
#define LL_SETTINGS_DAYCYCLE_H

#include "llsettingsbase.h"

class LLSettingsWater;
class LLSettingsSky;

typedef boost::shared_ptr<LLSettingsWater> LLSettingsWaterPtr_t;
typedef boost::shared_ptr<LLSettingsSky> LLSettingsSkyPtr_t;

class LLSettingsDay : public LLSettingsBase
{
public:
    static const std::string    SETTING_DAYLENGTH;
    static const std::string    SETTING_KEYID;
    static const std::string    SETTING_KEYNAME;
    static const std::string    SETTING_KEYKFRAME;
    static const std::string    SETTING_KEYHASH;
    static const std::string    SETTING_TRACKS;
    static const std::string    SETTING_FRAMES;

    static const S64            MINIMUM_DAYLENGTH;
    static const S64            DEFAULT_DAYLENGTH;
    static const S64            MAXIMUM_DAYLENGTH;

    static const S32            MINIMUM_DAYOFFSET;
    static const S32            DEFAULT_DAYOFFSET;
    static const S32            MAXIMUM_DAYOFFSET;

    static const S32            TRACK_WATER;
    static const S32            TRACK_MAX;
    static const S32            FRAME_MAX;

    typedef std::map<F32, LLSettingsBase::ptr_t>    CycleTrack_t;
    typedef std::vector<CycleTrack_t>               CycleList_t;
    typedef boost::shared_ptr<LLSettingsDay>        ptr_t;
    typedef std::vector<S64Seconds>                 TimeList_t;
    typedef std::vector<F32>                        KeyframeList_t;
    typedef std::pair<CycleTrack_t::iterator, CycleTrack_t::iterator> TrackBound_t;

    //---------------------------------------------------------------------
    LLSettingsDay(const LLSD &data);
    virtual ~LLSettingsDay() { };

    bool                        initialize();

    virtual ptr_t               buildClone() = 0;
    virtual LLSD                getSettings() const;

    //---------------------------------------------------------------------
    virtual std::string         getSettingType() const { return std::string("daycycle"); }

    // Settings status 
    virtual void                blend(const LLSettingsBase::ptr_t &other, F64 mix);

    static LLSD                 defaults();

    //---------------------------------------------------------------------

    KeyframeList_t              getTrackKeyframes(S32 track);
    TimeList_t                  getTrackTimes(S32 track);

    void                        setWaterAtTime(const LLSettingsWaterPtr_t &water, S64Seconds seconds);
    void                        setWaterAtKeyframe(const LLSettingsWaterPtr_t &water, F32 keyframe);

    void                        setSkyAtTime(const LLSettingsSkyPtr_t &sky, S64Seconds seconds, S32 track);
    void                        setSkyAtKeyframe(const LLSettingsSkyPtr_t &sky, F32 keyframe, S32 track);
        //---------------------------------------------------------------------
    void                        startDayCycle();

    LLSettingsSkyPtr_t          getCurrentSky() const
    {
        return mBlendedSky;
    }

    LLSettingsWaterPtr_t        getCurrentWater() const
    {
        return mBlendedWater;
    }

    virtual LLSettingsSkyPtr_t  getDefaultSky() const = 0;
    virtual LLSettingsWaterPtr_t getDefaultWater() const = 0;

    virtual LLSettingsSkyPtr_t  buildSky(LLSD) const = 0;
    virtual LLSettingsWaterPtr_t buildWater(LLSD) const = 0;

    virtual LLSettingsSkyPtr_t  getNamedSky(const std::string &) const = 0;
    virtual LLSettingsWaterPtr_t getNamedWater(const std::string &) const = 0;

    void    setInitialized(bool value = true) { mInitialized = value; }
    CycleTrack_t &              getCycleTrack(S32 track);

    virtual validation_list_t   getValidationList() const;
    static validation_list_t    validationList();

    S64Seconds                  getDayLength() const { return mDayLength; }
    void                        setDayLength(S64Seconds val ) { mDayLength = val; }
    S64Seconds                  getDayOffset() const { return mDayOffset; }
    void                        setDayOffset(S64Seconds val) { mDayOffset = val; }

protected:
    LLSettingsDay();

    virtual void                updateSettings();

    bool                        mInitialized;

private:
    LLSettingsBlender::ptr_t    mSkyBlender;    // convert to [] for altitudes 
    LLSettingsBlender::ptr_t    mWaterBlender;

    LLSettingsSkyPtr_t          mBlendedSky;
    LLSettingsWaterPtr_t        mBlendedWater;

    CycleList_t                 mDayTracks;

    F64Seconds                  mLastUpdateTime;

    S64Seconds mDayLength;
    S64Seconds mDayOffset;

    F32                         secondsToKeyframe(S64Seconds seconds);
    F64Seconds                  keyframeToSeconds(F32 keyframe);

    void                        parseFromLLSD(LLSD &data);

    static CycleTrack_t::iterator   getEntryAtOrBefore(CycleTrack_t &track, F32 keyframe);
    static CycleTrack_t::iterator   getEntryAtOrAfter(CycleTrack_t &track, F32 keyframe);

    TrackBound_t                getBoundingEntries(CycleTrack_t &track, F32 keyframe);
    TrackBound_t                getBoundingEntries(CycleTrack_t &track, F64Seconds time);

    void                        onSkyTransitionDone(S32 track, const LLSettingsBlender::ptr_t &blender);
    void                        onWaterTransitionDone(const LLSettingsBlender::ptr_t &blender);

};

#endif
