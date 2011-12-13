#include "cancellationDialog.h"
#include <QtGui>
CancellationDialog::CancellationDialog(QWidget *parent)
:QDialog(parent)
{
	label = new QLabel(tr("Threshold:"));
	m_pthreshold = new QDoubleSpinBox;
	m_pthreshold->setDecimals(3);

	QSpacerItem *spacerItem = new QSpacerItem(131, 31, QSizePolicy::Minimum, QSizePolicy::Expanding);


	okButton = new QPushButton(tr("OK"));
	cancelButton = new QPushButton(tr("Close"));

	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));


	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(label);
	layout->addWidget(m_pthreshold);


	QHBoxLayout *hlayout = new QHBoxLayout;
	hlayout->addItem(spacerItem);
	hlayout->addWidget(okButton);
	hlayout->addWidget(cancelButton);

	QVBoxLayout *vlayout = new QVBoxLayout;
	vlayout->addLayout(layout);
	vlayout->addLayout(hlayout);

	setLayout(vlayout);

	setWindowTitle(tr("Select a eigenvector"));
	setFixedHeight(sizeHint().height());
}


