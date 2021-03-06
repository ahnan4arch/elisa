/***************************************************************************
 *   Copyright 2014 Sujith Haridasan <sujith.haridasan@kdemail.net>        *
 *   Copyright 2014 Ashish Madeti <ashishmadeti@gmail.com>                 *
 *   Copyright 2016 Matthieu Gallien <mgallien@mgallien.fr>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef MEDIAPLAYER2PLAYER_H
#define MEDIAPLAYER2PLAYER_H

#include <QDBusAbstractAdaptor>
#include <QDBusObjectPath>
#include <QPointer>
#include <QUrl>

class PlayListControler;
class ManageAudioPlayer;
class ManageMediaPlayerControl;
class ManageHeaderBar;
class AudioWrapper;

class MediaPlayer2Player : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player") // Docs: http://specifications.freedesktop.org/mpris-spec/latest/Player_Interface.html

    Q_PROPERTY(QString PlaybackStatus READ PlaybackStatus)
    Q_PROPERTY(double Rate READ Rate WRITE setRate NOTIFY rateChanged)
    Q_PROPERTY(QVariantMap Metadata READ Metadata)
    Q_PROPERTY(double Volume READ Volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(qlonglong Position READ Position WRITE setPropertyPosition)
    Q_PROPERTY(double MinimumRate READ MinimumRate)
    Q_PROPERTY(double MaximumRate READ MaximumRate)
    Q_PROPERTY(bool CanGoNext READ CanGoNext)
    Q_PROPERTY(bool CanGoPrevious READ CanGoPrevious)
    Q_PROPERTY(bool CanPlay READ CanPlay)
    Q_PROPERTY(bool CanPause READ CanPause)
    Q_PROPERTY(bool CanControl READ CanControl)
    Q_PROPERTY(bool CanSeek READ CanSeek)
    Q_PROPERTY(int currentTrack READ currentTrack WRITE setCurrentTrack)
    Q_PROPERTY(int mediaPlayerPresent READ mediaPlayerPresent WRITE setMediaPlayerPresent)

public:
    explicit MediaPlayer2Player(PlayListControler *playListControler,
                                ManageAudioPlayer *manageAudioPlayer,
                                ManageMediaPlayerControl* manageMediaPlayerControl,
                                ManageHeaderBar * manageHeaderBar,
                                AudioWrapper *audioPlayer,
                                QObject* parent = nullptr);
    ~MediaPlayer2Player() override;

    QString PlaybackStatus() const;
    double Rate() const;
    QVariantMap Metadata() const;
    double Volume() const;
    qlonglong Position() const;
    double MinimumRate() const;
    double MaximumRate() const;
    bool CanGoNext() const;
    bool CanGoPrevious() const;
    bool CanPlay() const;
    bool CanPause() const;
    bool CanSeek() const;
    bool CanControl() const;
    int currentTrack() const;
    int mediaPlayerPresent() const;

Q_SIGNALS:
    void Seeked(qlonglong Position) const;

    void rateChanged(double newRate) const;
    void volumeChanged(double newVol) const;
    void next() const;
    void previous() const;
    void playPause() const;
    void stop() const;

public Q_SLOTS:

    void emitSeeked(int pos);

    void Next() const;
    void Previous() const;
    void Pause() const;
    void PlayPause();
    void Stop() const;
    void Play() const;
    void Seek(qlonglong Offset) const;
    void SetPosition(const QDBusObjectPath &trackId, qlonglong pos) const;
    void OpenUri(const QString &uri) const;

private Q_SLOTS:

    void playerSourceChanged();

    void playControlEnabledChanged();

    void skipBackwardControlEnabledChanged();

    void skipForwardControlEnabledChanged();

    void playerPlaybackStateChanged();

    void playerIsSeekableChanged();

    void audioPositionChanged();

    void audioDurationChanged();

    void playerVolumeChanged();

private:
    void signalPropertiesChange(const QString &property, const QVariant &value);

    void setMediaPlayerPresent(int status);
    void setRate(double newRate);
    void setVolume(double volume);
    void setPropertyPosition(int newPositionInMs);
    void setCurrentTrack(int newTrackPosition);

    QVariantMap getMetadataOfCurrentTrack();

    QVariantMap m_metadata;
    QString m_currentTrack;
    QString m_currentTrackId;
    double m_rate = 1.0;
    double m_volume = 0.0;
    int m_mediaPlayerPresent = 0;
    bool m_canPlay = false;
    bool m_canGoNext = false;
    bool m_canGoPrevious = false;
    qlonglong m_position = 0;
    PlayListControler *m_playListControler = nullptr;
    bool m_playerIsSeekableChanged = false;
    ManageAudioPlayer* m_manageAudioPlayer = nullptr;
    ManageMediaPlayerControl* m_manageMediaPlayerControl = nullptr;
    ManageHeaderBar * m_manageHeaderBar = nullptr;
    AudioWrapper *m_audioPlayer = nullptr;
};

#endif // MEDIAPLAYER2PLAYER_H
