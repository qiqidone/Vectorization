#ifndef _CANCELLATIONDIALOG_H
#define _CANCELLATIONDIALOG_H
#include <QDialog>
class QLabel;
class QPushButton;
class QDoubleSpinBox;
class QSpacerIterm;
class CancellationDialog:public QDialog
{
public:
	CancellationDialog(QWidget *parent = NULL);
	QDoubleSpinBox *m_pthreshold;

private:
	QPushButton *okButton;
	QPushButton *cancelButton;
	QLabel *label;
};


#endif

