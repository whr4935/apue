#ifndef ICONEDITORPLUGIN_H
#define ICONEDITORPLUGIN_H

#include <QtDesigner/QDesignerCustomWidgetInterface>

class iconeditorPlugin : public QObject, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" FILE "iconeditorplugin.json")
	Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
	iconeditorPlugin(QObject *parent = 0);

	bool isContainer() const;
	bool isInitialized() const;
	QIcon icon() const;
	QString domXml() const;
	QString group() const;
	QString includeFile() const;
	QString name() const;
	QString toolTip() const;
	QString whatsThis() const;
	QWidget *createWidget(QWidget *parent);
	void initialize(QDesignerFormEditorInterface *core);

private:
	bool initialized;
};

#endif // ICONEDITORPLUGIN_H
