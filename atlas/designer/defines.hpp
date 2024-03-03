//
// Created by kj16609 on 2/18/24.
//

#pragma once

#include <QDesignerCustomWidgetInterface>
#include <QObject>
#include <QSettings>
#include <QtUiPlugin/QDesignerExportWidget>

#define ATLAS_PROPERTY_SETTINGS                                                                                        \
                                                                                                                       \
  private:                                                                                                             \
                                                                                                                       \
	QString m_settings_namespace {};                                                                                   \
	QString m_settings_key {};                                                                                         \
                                                                                                                       \
  public:                                                                                                              \
                                                                                                                       \
	Q_PROPERTY( QString m_settings_namespace READ settingsNamespace WRITE setSettingsNamespace )                       \
	Q_PROPERTY( QString m_settings_key READ settingsKey WRITE setSettingsKey )                                         \
	void setSettingsNamespace( const QString& settingsNamespace )                                                      \
	{                                                                                                                  \
		m_settings_namespace = settingsNamespace;                                                                      \
	}                                                                                                                  \
	QString settingsNamespace() const                                                                                  \
	{                                                                                                                  \
		return m_settings_namespace;                                                                                   \
	}                                                                                                                  \
	void setSettingsKey( const QString& settingsKey )                                                                  \
	{                                                                                                                  \
		m_settings_key = settingsKey;                                                                                  \
	}                                                                                                                  \
	QString settingsKey() const                                                                                        \
	{                                                                                                                  \
		return m_settings_key;                                                                                         \
	}                                                                                                                  \
	QString fullSettingsKey() const                                                                                    \
	{                                                                                                                  \
		return m_settings_namespace + "/" + m_settings_key;                                                            \
	}

#define ATLAS_SETTINGS_WRAPPER( Name, QtType )                                                                         \
	class Name : public QtType, public AtlasSettingsInterfaceC                                                         \
	{                                                                                                                  \
		Q_OBJECT                                                                                                       \
                                                                                                                       \
	  public:                                                                                                          \
                                                                                                                       \
		Name( QWidget* parent ) : QtType( parent )                                                                     \
		{}                                                                                                             \
                                                                                                                       \
		ATLAS_PROPERTY_SETTINGS                                                                                        \
                                                                                                                       \
	  public slots:                                                                                                    \
		void saveSettings( QSettings& settings );                                                                      \
		void loadSettings( QSettings& settings );                                                                      \
	}

#define XML_BEGIN( name ) "<" #name ">"
#define XML_END( name ) "</" #name ">"
#define XML_WRAP( name, val ) XML_BEGIN( name ) #val XML_END( name )
#define XML_WRAP_TEXT( name, text ) XML_BEGIN( name ) text XML_END( name )

#define XML_CPP_LANGUAGE "<ui language=\"c++\">"
#define XML_WIDGET_CLASS( class_name ) "<widget class=\"" #class_name "\">"
#define XML_PROPERTY_GEOMETRY "<property name=\"geometry\">"
#define XML_RECT XML_BEGIN( rect )
#define XML_X( val ) XML_WRAP( x, val )
#define XML_Y( val ) XML_WRAP( y, val )
#define XML_WIDTH( val ) XML_WRAP( width, val )
#define XML_HEIGHT( val ) XML_WRAP( height, val )
#define XML_END_RECT XML_END( rect )
#define XML_END_PROPERTY_GEOMETRY XML_END( property )
#define XML_END_WIDGET_CLASS XML_END( widget )

#define XML_CUSTOM_WIDGETS XML_BEGIN( customwidgets )
#define XML_CUSTOM_WIDGET XML_BEGIN( customwidget )
#define XML_CLASS( class_name ) XML_WRAP( class, class_name )
#define XML_EXTENDS( class_name ) XML_WRAP( extends, class_name )
#define XML_HEADER( header_name ) XML_WRAP_TEXT( header, header_name )
#define XML_END_CUSTOM_WIDGET XML_END( customwidget )
#define XML_END_CUSTOM_WIDGETS XML_END( customwidgets )

#define XML_WIDGET( class_name )                                                                                       \
	XML_CPP_LANGUAGE                                                                                                   \
	XML_WIDGET_CLASS( class_name )                                                                                     \
	XML_PROPERTY_GEOMETRY                                                                                              \
	XML_RECT                                                                                                           \
	XML_X( 0 )                                                                                                         \
	XML_Y( 0 )                                                                                                         \
	XML_WIDTH( 100 )                                                                                                   \
	XML_HEIGHT( 100 )                                                                                                  \
	XML_END_RECT                                                                                                       \
	XML_END_PROPERTY_GEOMETRY                                                                                          \
	XML_END_WIDGET_CLASS                                                                                               \
	XML_CUSTOM_WIDGETS                                                                                                 \
	XML_CUSTOM_WIDGET                                                                                                  \
	XML_CLASS( class_name )                                                                                            \
	XML_EXTENDS( QWidget )                                                                                             \
	XML_HEADER( "designer/primitives/" #class_name ".hpp" )                                                            \
	XML_END_CUSTOM_WIDGET                                                                                              \
	XML_END_CUSTOM_WIDGETS                                                                                             \
	XML_END( ui )

#define ATLAS_SETTINGS_INTERFACE( Name, InterfaceName )                                                                \
	class InterfaceName : public QWidget, public QDesignerCustomWidgetInterface                                        \
	{                                                                                                                  \
		Q_OBJECT                                                                                                       \
		Q_INTERFACES( QDesignerCustomWidgetInterface )                                                                 \
                                                                                                                       \
		bool initalized { false };                                                                                     \
                                                                                                                       \
	  public:                                                                                                          \
                                                                                                                       \
		explicit InterfaceName( QWidget* parent = nullptr ) : QWidget( parent )                                        \
		{}                                                                                                             \
                                                                                                                       \
		QString name() const override                                                                                  \
		{                                                                                                              \
			return #Name;                                                                                              \
		}                                                                                                              \
                                                                                                                       \
		QString group() const override                                                                                 \
		{                                                                                                              \
			return "Atlas Widgets";                                                                                    \
		}                                                                                                              \
                                                                                                                       \
		QString toolTip() const override                                                                               \
		{                                                                                                              \
			return "Widget for setting toggle settings";                                                               \
		}                                                                                                              \
                                                                                                                       \
		QString whatsThis() const override                                                                             \
		{                                                                                                              \
			return "Widget for setting toggle settings";                                                               \
		}                                                                                                              \
                                                                                                                       \
		QString includeFile() const override                                                                           \
		{                                                                                                              \
			return "designer/primitives/" #Name ".hpp";                                                                \
		}                                                                                                              \
                                                                                                                       \
		QIcon icon() const override                                                                                    \
		{                                                                                                              \
			return QIcon();                                                                                            \
		}                                                                                                              \
                                                                                                                       \
		bool isContainer() const override                                                                              \
		{                                                                                                              \
			return false;                                                                                              \
		}                                                                                                              \
                                                                                                                       \
		QWidget* createWidget( QWidget* parent ) override                                                              \
		{                                                                                                              \
			return new Name( parent );                                                                                 \
		}                                                                                                              \
                                                                                                                       \
		void initialize( [[maybe_unused]] QDesignerFormEditorInterface* core ) override                                \
		{                                                                                                              \
			if ( initalized )                                                                                          \
			{                                                                                                          \
				return;                                                                                                \
			}                                                                                                          \
                                                                                                                       \
			initalized = true;                                                                                         \
		}                                                                                                              \
                                                                                                                       \
		bool isInitialized() const override                                                                            \
		{                                                                                                              \
			return initalized;                                                                                         \
		}                                                                                                              \
                                                                                                                       \
		QString domXml() const override                                                                                \
		{                                                                                                              \
			return XML_WIDGET( Name );                                                                                 \
		}                                                                                                              \
	}
