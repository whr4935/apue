#ifndef BASICEFFECTSPLUGIN_H
#define BASICEFFECTSPLUGIN_H

#include <QObject>

#include "../textart/textartinterface.h"

class BasicEffectsPlugin : public QObject, public TextArtInterface
{
    Q_OBJECT
    Q_INTERFACES(TextArtInterface)
    Q_PLUGIN_METADATA(IID "aaa" FILE "")

public:
    QStringList effects() const;
    QPixmap applyEffect(const QString &effect, const QString &text,
                        const QFont &font, const QSize &size,
                        const QPen &pen, const QBrush &brush);
};

#endif
