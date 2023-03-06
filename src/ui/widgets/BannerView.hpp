//
// Created by kj16609 on 3/1/23.
//

#ifndef HYDRUS95_BANNERVIEW_HPP
#define HYDRUS95_BANNERVIEW_HPP

#include <filesystem>
#include <QLabel>

class BannerView : public QLabel
{
	Q_OBJECT

	Q_DISABLE_COPY_MOVE(BannerView)

	bool edit_allowed {false};
	std::filesystem::path m_banner {":/banner/placeholder.jpg"};

	public:
	explicit BannerView( QWidget* parent = nullptr );
	~BannerView() override;

	//! Sets if the banner can be changed by dragging into it.
	void setEditable(const bool);

	//! Sets the banner image
	void setBanner(const std::filesystem::path& banner);

	//! Returns the current path for the banner image
	std::filesystem::path path() const;

	private:
	//! Loads m_banner into the QPixmap
	void loadImage();

	protected:
	friend class TestBannerView;
	// Qt overrides
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
};


#endif	//HYDRUS95_BANNERVIEW_HPP
