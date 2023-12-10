//
// Created by kj16609 on 3/25/23.
//

#pragma once
#ifndef ATLAS_PROGRESSBARDIALOG_HPP
#define ATLAS_PROGRESSBARDIALOG_HPP

#include <QDialog>

QT_BEGIN_NAMESPACE

namespace Ui
{
	class ProgressBarDialog;
}

QT_END_NAMESPACE

class ProgressBarDialog final : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE( ProgressBarDialog )

  public:

	explicit ProgressBarDialog( QWidget* parent = nullptr );
	~ProgressBarDialog() override;

	void showSubProgress( const bool show );

	void setMax( const int max );
	void setValue( const int value );
	void setText( const QString text );

	void setSubMax( const int max );
	void setSubValue( const int value );
	void setSubText( const QString text );

  private:

	Ui::ProgressBarDialog* ui;
};

#endif //ATLAS_PROGRESSBARDIALOG_HPP
