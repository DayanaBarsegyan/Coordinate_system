#ifndef COORDSYSTEM_H
#define COORDSYSTEM_H

#include <QWidget>
#include <QMouseEvent>

class QTableWidgetItem;
class QTableWidget;
class QPushButton;
class QPoint;

class CoordSystem:public QWidget
{
    Q_OBJECT;
public:
    CoordSystem();
    ~CoordSystem();
private:
    void set_connections();
    void set_layout();
    void set_style();
    void draw_axis(QPainter* painter);
    void draw_points_func(QPainter* painter);
    void draw_lines_func(QPainter* painter);
    void make_array_of_points();
    void sort_by_x();
    void sort_by_y();
    void button_setup() const;
    bool points_are_even() const;
    bool is_full_row(int row) const;
    bool is_empty_row(int row) const;

private slots:
    void add_new_point();
    void clean_last_point();
    void cleartable();

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent*);

private:
    QTableWidget* table;
    QPushButton* draw_points;
    QPushButton* draw_lines;
    QPushButton* clear;
    QPushButton* add_point;
    QPushButton* clear_the_last_point;
    QPoint* points;
    int count;
    int count_of_real_points;
    const int x0 = 560;
    const int y0 = 234;
};

#endif // COORDSYSTEM_H
