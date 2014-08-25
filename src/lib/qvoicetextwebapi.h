/* Copyright (c) 2014 Tasuku Suzuki.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Tasuku Suzuki nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL TASUKU SUZUKI BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef QVOICETEXTWEBAPI_H
#define QVOICETEXTWEBAPI_H

#include "qtvoicetextwebapi_global.h"
#include <QtCore/QObject>

class QNetworkAccessManager;

class QTVOICETEXT_EXPORT QVoiceTextWebAPI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QByteArray apiKey READ apiKey WRITE setApiKey NOTIFY apiKeyChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_ENUMS(Speaker)
    Q_PROPERTY(Speaker speaker READ speaker WRITE setSpeaker NOTIFY speakerChanged)
    Q_ENUMS(Emotion)
    Q_PROPERTY(Emotion emotion READ emotion WRITE setEmotion NOTIFY emotionChanged)
    Q_PROPERTY(int emotionLevel READ emotionLevel WRITE setEmotionLevel NOTIFY emotionLevelChanged)
    Q_PROPERTY(int pitch READ pitch WRITE setPitch NOTIFY pitchChanged)
    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(QNetworkAccessManager *networkAccessManager READ networkAccessManager WRITE setNetworkAccessManager NOTIFY networkAccessManagerChanged)
public:
    enum Speaker {
        Show,
        Haruka,
        Hikari,
        Takeru,
    };
    enum Emotion {
        NoEmotion,
        Happiness,
        Anger,
        Sadness,
    };

    explicit QVoiceTextWebAPI(QObject *parent = 0);
    ~QVoiceTextWebAPI();

    QByteArray apiKey() const;
    QString text() const;
    Speaker speaker() const;
    Emotion emotion() const;
    int emotionLevel() const;
    int pitch() const;
    int speed() const;
    int volume() const;
    QNetworkAccessManager *networkAccessManager() const;

public slots:
    void setApiKey(const QByteArray &apiKey);
    void setText(const QString &text);
    void setSpeaker(Speaker speaker);
    void setEmotion(Emotion emotion);
    void setEmotionLevel(int emotionLevel);
    void setPitch(int pitch);
    void setSpeed(int speed);
    void setVolume(int volume);
    void setNetworkAccessManager(QNetworkAccessManager *networkAccessManager);

    void play();

signals:
    void apiKeyChanged(const QByteArray &apiKey);
    void textChanged(const QString &text);
    void speakerChanged(Speaker speaker);
    void emotionChanged(Emotion emotion);
    void emotionLevelChanged(int emotionLevel);
    void pitchChanged(int pitch);
    void speedChanged(int speed);
    void volumeChanged(int volume);
    void networkAccessManagerChanged(QNetworkAccessManager *networkAccessManager);

private:
    class Private;
    Private *d;
};

#endif // QVOICETEXTWEBAPI_H
