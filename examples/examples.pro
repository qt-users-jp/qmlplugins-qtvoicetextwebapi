TEMPLATE = subdirs
!isEmpty(QT.voicetextwebapi.name) {
    !isEmpty(QT.widgets.name): SUBDIRS += widget
    !isEmpty(QT.quick.name): SUBDIRS += quick
}
