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

import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.1
import QtQuick.Layouts 1.1
import QtVoiceTextWebAPI 0.1

ApplicationWindow {
    title: qsTr("VoiceText")
    width: 300
    height: 300

    VoiceTextWebAPI {
        id: voiceText
        apiKey: apiKey.text
        text: text.text
        speaker: speaker.currentIndex
        emotion: emotion.currentIndex
        emotionLevel: parseInt(emotionLevel.current.text)
        pitch: pitch.value
        speed: speed.value
        volume: volume.value
    }

    GridLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        rowSpacing: 5
        columns: 2

        Label {
            text: qsTr('API Key:')
        }
        RowLayout {
            TextField {
                id: apiKey
                Layout.fillWidth: true
                echoMode: TextInput.Password
                text: 'wfez2l7muamw7qih'
            }
            Button {
                text: qsTr('Register...')
                onClicked: Qt.openUrlExternally('https://cloud.voicetext.jp/webapi/api_keys/new')
            }
        }

        Label {
            text: qsTr('Text:')
        }
        RowLayout {
            TextField {
                id: text
                focus: true
                text: 'おはようございます'
                placeholderText: 'おはようございます'
                Layout.fillWidth: true
                Keys.onReturnPressed: voiceText.play()
            }
            Button {
                text: qsTr('Speak')
                onClicked: voiceText.play()
            }
        }

        Label {
            text: qsTr('Speaker')
        }
        ComboBox {
            id: speaker
            currentIndex: 0
            model: ListModel {
                ListElement { text: QT_TR_NOOP('Show(male)') }
                ListElement { text: QT_TR_NOOP('Haruka(female)') }
                ListElement { text: QT_TR_NOOP('Hikari(female)') }
                ListElement { text: QT_TR_NOOP('Takeru(male)') }
            }
            Layout.fillWidth: true
        }

        Label {
            text: qsTr('Emotion')
        }
        RowLayout {
            enabled: speaker.currentIndex > 0
            ComboBox {
                id: emotion
                currentIndex: 0
                model: ListModel {
                    ListElement { text: QT_TR_NOOP('No emotion') }
                    ListElement { text: QT_TR_NOOP('Happiness') }
                    ListElement { text: QT_TR_NOOP('Anger') }
                    ListElement { text: QT_TR_NOOP('Sadness') }
                }
                Layout.fillWidth: true
            }
            ExclusiveGroup {
                id: emotionLevel
            }

            RadioButton {
                text: '1'
                checked: true
                exclusiveGroup: emotionLevel
            }
            RadioButton {
                text: '2'
                exclusiveGroup: emotionLevel
            }
        }

        Label {
            text: qsTr('Pitch')
        }
        RowLayout {
            Slider {
                id: pitch
                minimumValue: 50
                maximumValue: 200
                stepSize: 5
                value: 100
                Layout.fillWidth: true
            }
            Label {
                text: pitch.value
            }
            Layout.fillWidth: true
        }

        Label {
            text: qsTr('Speed')
        }
        RowLayout {
            Slider {
                id: speed
                minimumValue: 50
                maximumValue: 500
                stepSize: 5
                value: 100
                Layout.fillWidth: true
            }
            Label {
                text: speed.value
            }
            Layout.fillWidth: true
        }

        Label {
            text: qsTr('Volume')
        }
        RowLayout {
            Slider {
                id: volume
                minimumValue: 50
                maximumValue: 200
                stepSize: 5
                value: 100
                Layout.fillWidth: true
            }
            Label {
                text: volume.value
            }
            Layout.fillWidth: true
        }
    }
}
