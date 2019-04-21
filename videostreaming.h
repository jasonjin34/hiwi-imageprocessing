#ifndef VIDEOSTREAMING_H
#define VIDEOSTREAMING_H

#include <QWidget>

namespace Ui {
class Videostreaming;
}

class Videostreaming : public QWidget
{
    Q_OBJECT

public:
    explicit Videostreaming(QWidget *parent = nullptr);
    ~Videostreaming();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Videostreaming *ui;
};

#endif // VIDEOSTREAMING_H
