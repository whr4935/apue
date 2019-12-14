#include "iconeditor.h"

#include <QtCore/QtPlugin>
#include "iconeditorplugin.h"


iconeditorPlugin::iconeditorPlugin(QObject *parent)
	: QObject(parent)
{
	initialized = false;
}

void iconeditorPlugin::initialize(QDesignerFormEditorInterface * /*core*/)
{
	if (initialized)
		return;

	initialized = true;
}

bool iconeditorPlugin::isInitialized() const
{
	return initialized;
}

QWidget *iconeditorPlugin::createWidget(QWidget *parent)
{
	return new IconEditor(parent);
}

QString iconeditorPlugin::name() const
{
	return "iconeditor";
}

QString iconeditorPlugin::group() const
{
	return "My Plugins";
}

QIcon iconeditorPlugin::icon() const
{
	return QIcon(":/images/mouse.png");
}

QString iconeditorPlugin::toolTip() const
{
	return QString();
}

QString iconeditorPlugin::whatsThis() const
{
	return QString();
}

bool iconeditorPlugin::isContainer() const
{
	return false;
}

QString iconeditorPlugin::domXml() const
{
	return "<widget class=\"iconeditor\" name=\"iconeditor\">\n"
		" <property name=\"geometry\">\n"
		"  <rect>\n"
		"   <x>0</x>\n"
		"   <y>0</y>\n"
		"   <width>100</width>\n"
		"   <height>100</height>\n"
		"  </rect>\n"
		" </property>\n"
		"</widget>\n";
}

QString iconeditorPlugin::includeFile() const
{
	return "iconeditor.h";
}

