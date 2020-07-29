/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * ( at your option ) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import org.videolan.vlc 0.1

import "qrc:///widgets/" as Widgets
import "qrc:///style/"


Rectangle {
    id: plitem

    property var plmodel

    signal itemClicked(int button, int modifier)
    signal itemDoubleClicked(int keys, int modifier)
    signal dragStarting()

    property alias hovered: mouse.containsMouse

    property var dragitem: null
    signal dropedMovedAt(int target, var drop)

    property int leftPadding: 0
    property int rightPadding: 0

    property VLCColors _colors: VLCStyle.colors

    // Should the cover be displayed
    //property alias showCover: cover.visible

    // This item will become the parent of the dragged item during the drag operation
    //property alias draggedItemParent: draggable_item.draggedItemParent

    height: Math.max( VLCStyle.fontHeight_normal, VLCStyle.icon_normal ) + VLCStyle.margin_xsmall

    property bool dropVisible: false

    Connections {
        target: root

        onSetItemDropIndicatorVisible: {
            if (index === model.index)
            {
                dropVisible = isVisible
            }
        }
    }

    Rectangle {
        z: 2
        width: parent.width
        height: 2
        anchors.top: parent.top
        antialiasing: true
        visible: dropVisible
        color: _colors.accent
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true

        acceptedButtons: acceptedButtons | Qt.RightButton

        onClicked:{
            plitem.itemClicked(mouse.button, mouse.modifiers);
        }
        onDoubleClicked: {
            if (mouse.button !== Qt.RightButton)
                plitem.itemDoubleClicked(mouse.buttons, mouse.modifiers);
        }

        drag.target: dragItem

        property bool __rightButton : false

        Connections {
            target: mouse.drag
            onActiveChanged: {
                if (mouse.__rightButton)
                    return
                if (target.active) {
                    plitem.dragStarting()
                    dragItem.model = model
                    dragItem.count = plmodel.getSelection().length
                    dragItem.visible = true
                } else {
                    dragItem.Drag.drop()
                    dragItem.visible = false
                }
            }
        }

        onPressed:  {
            if (mouse.button === Qt.RightButton)
            {
                __rightButton = true
                return
            }
            else
                __rightButton = false
        }

        onPositionChanged: {
            if (dragItem.visible)
            {
                var pos = this.mapToGlobal( mouseX, mouseY)
                dragItem.updatePos(pos.x + VLCStyle.dp(15), pos.y)
            }
        }

        Rectangle {
            color: _colors.bg
            anchors.fill: parent
            visible: model.isCurrent && !plitem.hovered && !model.selected
        }

        RowLayout {
            id: content
            anchors {
                fill: parent
                leftMargin: plitem.leftPadding
                rightMargin: plitem.rightPadding
            }

            Item {
                Layout.preferredHeight: VLCStyle.icon_normal
                Layout.preferredWidth: VLCStyle.icon_normal

                DropShadow {
                    id: effect
                    anchors.fill: artwork
                    source: artwork
                    radius: 8
                    samples: 17
                    color: _colors.glowColorBanner
                    visible: artwork.visible
                    spread: 0.1
                }

                Image {
                    id: artwork
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    source: (model.artwork && model.artwork.toString()) ? model.artwork : VLCStyle.noArtCover
                    visible: !statusIcon.visible
                }

                Widgets.IconLabel {
                    id: statusIcon
                    anchors.fill: parent
                    visible: (model.isCurrent && text !== "")
                    width: height
                    height: VLCStyle.icon_normal
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: _colors.accent
                    text: player.playingState === PlayerController.PLAYING_STATE_PLAYING ? VLCIcons.volume_high :
                                                    player.playingState === PlayerController.PLAYING_STATE_PAUSED ? VLCIcons.pause : ""
                }
            }

            Column {
                Layout.fillWidth: true
                Layout.leftMargin: VLCStyle.margin_large

                Widgets.ListLabel {
                    id: textInfo

                    font.weight: model.isCurrent ? Font.Bold : Font.Normal
                    text: model.title
                    color: _colors.text
                }

                Widgets.ListSubtitleLabel {
                    id: textArtist

                    font.weight: model.isCurrent ? Font.DemiBold : Font.Normal
                    text: (model.artist ? model.artist : i18n.qtr("Unknown Artist"))
                    color: _colors.text
                }
            }

            Widgets.ListLabel {
                id: textDuration
                Layout.rightMargin: VLCStyle.margin_xsmall
                Layout.preferredWidth: durationMetric.width
                text: model.duration
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: _colors.text

                TextMetrics {
                    id: durationMetric
                    font.pixelSize: VLCStyle.fontSize_normal
                    text: "-00:00-"
                }
            }
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            DropArea {
                id: higherDropArea
                Layout.fillWidth: true
                Layout.preferredHeight: parent.height / 2

                onEntered: {
                    var delta = drag.source.model.index - model.index
                    if(delta === 0 || delta === -1)
                        return

                    dropVisible = true
                }
                onExited: {
                    var delta = drag.source.model.index - model.index
                    if(delta === 0 || delta === -1)
                        return

                    dropVisible = false
                }
                onDropped: {
                    var delta = drag.source.model.index - model.index
                    if(delta === 0 || delta === -1)
                        return

                    plitem.dropedMovedAt(model.index, drop)
                    dropVisible = false
                }
            }

            DropArea {
                id: lowerDropArea
                Layout.fillWidth: true
                Layout.fillHeight: true

                // not visible on the last item since PLItemFooter handles it
                visible: model.index !== plitem.plmodel.count - 1

                onEntered: {
                    var delta = drag.source.model.index - model.index
                    if(delta === 0 || delta === 1)
                        return


                    root.setItemDropIndicatorVisible(model.index + 1, true, true);
                }
                onExited: {
                    var delta = drag.source.model.index - model.index
                    if(delta === 0 || delta === 1)
                        return

                    root.setItemDropIndicatorVisible(model.index + 1, false, true);
                }
                onDropped: {
                    var delta = drag.source.model.index - model.index
                    if(delta === 0 || delta === 1)
                        return

                    plitem.dropedMovedAt(model.index + 1, drop)
                    root.setItemDropIndicatorVisible(model.index + 1, false, true);
                }
            }

            PLItemFooter {
               Layout.fillWidth: true
               Layout.fillHeight: true

               visible: !lowerDropArea.visible

               dropIndicatorBottom: true
               onDropURLAtEnd: mainPlaylistController.insert(plitem.plmodel.count, urlList)
               onMoveAtEnd: plitem.plmodel.moveItemsPost(plitem.plmodel.getSelection(), plitem.plmodel.count - 1)
               listCount: plitem.plmodel.count
            }
        }
    }
}
