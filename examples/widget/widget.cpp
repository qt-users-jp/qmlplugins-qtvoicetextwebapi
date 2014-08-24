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

#include "widget.h"
#include "ui_widget.h"

#include <QtWidgets/QButtonGroup>
#include <QtVoiceTextWebAPI/QVoiceTextWebAPI>

class Widget::Private
{
public:
    Private(Widget *parent);

private:
    Widget *q;
    Ui::Widget ui;
    QVoiceTextWebAPI voiceText;
};

Widget::Private::Private(Widget *parent)
    : q(parent)
{
    ui.setupUi(q);
    connect(ui.apiKey, &QLineEdit::textChanged, [&](const QString &text) {
        voiceText.setApiKey(text.toUtf8());
    });
    ui.apiKey->setText(QStringLiteral("wfez2l7muamw7qih"));
    connect(ui.text, &QLineEdit::textChanged, [&](const QString &text) {
        voiceText.setText(text);
    });
    connect(ui.text, &QLineEdit::returnPressed, &voiceText, &QVoiceTextWebAPI::play);
    ui.text->setText("コンパイルが終わりました");
    ui.text->selectAll();
    ui.text->setFocus();
    connect(ui.speaker, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [&](int index) {
        voiceText.setSpeaker(static_cast<QVoiceTextWebAPI::Speaker>(index));
    });
    connect(ui.emotion, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [&](int index) {
        voiceText.setEmotion(static_cast<QVoiceTextWebAPI::Emotion>(index));
    });

    QButtonGroup *emotionLevel = new QButtonGroup(q);
    emotionLevel->addButton(ui.emotionLevel1);
    emotionLevel->addButton(ui.emotionLevel2);
    connect(emotionLevel, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), [&, emotionLevel]() {
        voiceText.setEmotionLevel(emotionLevel->checkedId() + 1);
    });

    connect(ui.pitch, &QDial::valueChanged, [&](int value) {
        voiceText.setPitch(value);
    });
    connect(ui.speed, &QDial::valueChanged, [&](int value) {
        voiceText.setSpeed(value);
    });
    connect(ui.volume, &QDial::valueChanged, [&](int value) {
        voiceText.setVolume(value);
    });
    connect(ui.play, &QPushButton::clicked, &voiceText, &QVoiceTextWebAPI::play);
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , d(new Private(this))
{
}

Widget::~Widget()
{
    delete d;
}
