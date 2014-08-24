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

#include "qvoicetextwebapi.h"

#include <QtCore/QDebug>
#include <QtCore/QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtMultimedia/QAudioOutput>

struct Chunk {
    char id[4];
    qint32 size;
};

struct RiffHeader : Chunk {
    char wave[4];
};

struct WavFormat : Chunk {
    qint16 audioFormat;
    qint16 numChannels;
    qint32 sampleRate;
    qint32 byteRate;
    qint16 blockAlign;
    qint16 bitsPerSample;
};

struct WavData : Chunk {
};

struct WavHeader {
    RiffHeader riffHeader;
    WavFormat wavFormat;
    WavData wavData;
};

class QVoiceTextWebAPI::Private : public QObject
{
    Q_OBJECT
public:
    Private(QVoiceTextWebAPI *parent);
    ~Private();

    void play();
    void parseHeader();
private:
    QVoiceTextWebAPI *q;
    QNetworkAccessManager networkAccessManager;

public:
    QByteArray apiKey;
    QString text;
    QVoiceTextWebAPI::Speaker speaker;
    QVoiceTextWebAPI::Emotion emotion;
    int emotionLevel;
    int pitch;
    int speed;
    int volume;
    WavHeader wavHeader;
};

QVoiceTextWebAPI::Private::Private(QVoiceTextWebAPI *parent)
    : q(parent)
    , apiKey()
    , text()
    , speaker(QVoiceTextWebAPI::Show)
    , emotion(QVoiceTextWebAPI::NoEmotion)
    , emotionLevel(1)
    , pitch(100)
    , speed(100)
    , volume(100)
{
}

QVoiceTextWebAPI::Private::~Private()
{
}

void QVoiceTextWebAPI::Private::play()
{
    if (apiKey.isEmpty()) {
        qWarning() << "apiKey is empty.";
        return;
    }
    if (text.isEmpty()) {
        qWarning() << "text is empty.";
        return;
    }
    QNetworkRequest request(QUrl(QStringLiteral("https://api.voicetext.jp/v1/tts")));
    request.setRawHeader("Authorization", "Basic " + apiKey.toBase64());
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/x-www-form-urlencoded"));
//    request.setRawHeader("Accept-Encoding", "qcompress");

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("text"), text);

    QString speakerValue;
    switch (speaker) {
    case Show:   speakerValue = QStringLiteral("show");   break;
    case Haruka: speakerValue = QStringLiteral("haruka"); break;
    case Hikari: speakerValue = QStringLiteral("hikari"); break;
    case Takeru: speakerValue = QStringLiteral("takeru"); break;
    }
    query.addQueryItem(QStringLiteral("speaker"), speakerValue);

    if (speaker != Show) {
        QString emotionValue;
        switch (emotion) {
        case NoEmotion: break;
        case Happiness: emotionValue = QStringLiteral("happiness"); break;
        case Anger:     emotionValue = QStringLiteral("anger");     break;
        case Sadness:   emotionValue = QStringLiteral("sadness");   break;
        }
        if (!emotionValue.isNull()) {
            query.addQueryItem(QStringLiteral("emotion"), emotionValue);
            query.addQueryItem(QStringLiteral("emotion_level"), QString::number(emotionLevel));
        }
    }

    query.addQueryItem(QStringLiteral("pitch"), QString::number(pitch));
    query.addQueryItem(QStringLiteral("speed"), QString::number(speed));
    query.addQueryItem(QStringLiteral("volume"), QString::number(volume));
//    qDebug() << query.toString();
    QNetworkReply *reply = networkAccessManager.post(request, query.toString().toUtf8());
    connect(reply, &QNetworkReply::readyRead, this, &QVoiceTextWebAPI::Private::parseHeader);
//    connect(reply, &QNetworkReply::downloadProgress, [&](qint64 bytesReceived, qint64 bytesTotal) {
//        qDebug() << bytesReceived << bytesTotal;
//    });
}

void QVoiceTextWebAPI::Private::parseHeader()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->read(reinterpret_cast<char *>(&wavHeader), sizeof(WavHeader));
    QAudioFormat audioFormat;
    if (qstrncmp(wavHeader.riffHeader.id, "RIFF", 4) != 0) {
        qCritical() << "RIFF id error:" << QByteArray(wavHeader.riffHeader.id, 4);
        return;
    }
    if (qstrncmp(wavHeader.riffHeader.wave, "WAVE", 4) != 0) {
        qCritical() << "RIFF wave error:" << QByteArray(wavHeader.riffHeader.wave, 4);
        return;
    }
    if (qstrncmp(wavHeader.wavFormat.id, "fmt ", 4) != 0) {
        qCritical() << "fmt id error:" << QByteArray(wavHeader.wavFormat.id, 4);
        return;
    }
    if (qstrncmp(wavHeader.wavData.id, "data", 4) != 0) {
        qCritical() << "data id error:" << QByteArray(wavHeader.wavData.id, 4);
        return;
    }
    audioFormat.setSampleRate(wavHeader.wavFormat.sampleRate);
    audioFormat.setChannelCount(wavHeader.wavFormat.numChannels);
    audioFormat.setSampleSize(wavHeader.wavFormat.bitsPerSample);
    switch (wavHeader.wavFormat.bitsPerSample) {
    case 8:
        audioFormat.setSampleType(QAudioFormat::UnSignedInt);
        break;
    case 16:
    case 32:
        audioFormat.setSampleType(QAudioFormat::SignedInt);
        break;
    }

    disconnect(reply, &QNetworkReply::readyRead, this, &QVoiceTextWebAPI::Private::parseHeader);

    QAudioOutput *output = new QAudioOutput(audioFormat, this);
//    int size = reply->header(QNetworkRequest::ContentLengthHeader).toInt() - sizeof(WavHeader);
//    qDebug() << size << size / audioFormat.channelCount() * 8 / audioFormat.sampleSize() / audioFormat.sampleRate();
//    qDebug() << wavHeader.wavFormat.numChannels << wavHeader.wavFormat.byteRate << wavHeader.wavFormat.blockAlign << wavHeader.wavFormat.sampleRate << wavHeader.wavFormat.bitsPerSample;
    output->start(reply);
    connect(output, &QAudioOutput::stateChanged, [reply, output](QAudio::State state) {
        switch (state) {
        case QAudio::IdleState:
            if (reply->isFinished()) {
                output->stop();
            }
            break;
        case QAudio::StoppedState:
            output->deleteLater();
            break;
        default:
            break;
        }
    });
}

QVoiceTextWebAPI::QVoiceTextWebAPI(QObject *parent)
    : QObject(parent)
    , d(new Private(this))
{
}

QVoiceTextWebAPI::~QVoiceTextWebAPI()
{
    delete d;
}

QByteArray QVoiceTextWebAPI::apiKey() const
{
    return d->apiKey;
}

void QVoiceTextWebAPI::setApiKey(const QByteArray &apiKey)
{
    if (d->apiKey == apiKey) return;
    d->apiKey = apiKey;
    emit apiKeyChanged(apiKey);
}

QString QVoiceTextWebAPI::text() const
{
    return d->text;
}

void QVoiceTextWebAPI::setText(const QString &text)
{
    if (d->text == text) return;
    d->text = text;
    emit textChanged(text);
}

QVoiceTextWebAPI::Speaker QVoiceTextWebAPI::speaker() const
{
    return d->speaker;
}

void QVoiceTextWebAPI::setSpeaker(QVoiceTextWebAPI::Speaker speaker)
{
    if (d->speaker == speaker) return;
    d->speaker = speaker;
    emit speakerChanged(speaker);
}

QVoiceTextWebAPI::Emotion QVoiceTextWebAPI::emotion() const
{
    return d->emotion;
}

void QVoiceTextWebAPI::setEmotion(QVoiceTextWebAPI::Emotion emotion)
{
    if (d->emotion == emotion) return;
    d->emotion = emotion;
    emit emotionChanged(emotion);
}

int QVoiceTextWebAPI::emotionLevel() const
{
    return d->emotionLevel;
}

void QVoiceTextWebAPI::setEmotionLevel(int emotionLevel)
{
    emotionLevel = qBound(1, emotionLevel, 2);
    if (d->emotionLevel == emotionLevel) return;
    d->emotionLevel = emotionLevel;
    emit emotionLevelChanged(emotionLevel);
}

int QVoiceTextWebAPI::pitch() const
{
    return d->pitch;
}

void QVoiceTextWebAPI::setPitch(int pitch)
{
    pitch = qBound(50, pitch, 200);
    if (d->pitch == pitch) return;
    d->pitch = pitch;
    emit pitchChanged(pitch);
}

int QVoiceTextWebAPI::speed() const
{
    return d->speed;
}

void QVoiceTextWebAPI::setSpeed(int speed)
{
    speed = qBound(50, speed, 400);
    if (d->speed == speed) return;
    d->speed = speed;
    emit speedChanged(speed);
}

int QVoiceTextWebAPI::volume() const
{
    return d->volume;
}

void QVoiceTextWebAPI::setVolume(int volume)
{
    volume = qBound(50, volume, 200);
    if (d->volume == volume) return;
    d->volume = volume;
    emit volumeChanged(volume);
}

void QVoiceTextWebAPI::play()
{
    d->play();
}

#include "qvoicetextwebapi.moc"
