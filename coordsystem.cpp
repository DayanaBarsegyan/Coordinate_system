#include "coordsystem.h"

#include <QTableWidget>
#include <QHBoxLayout>
#include <QPainter>
#include <QPushButton>
#include <QPoint>
#include <QTableWidgetItem>
#include <QColor>
#include <ctime>
#include <QMessageBox>
#include <QLabel>
#include <QPixmap>

CoordSystem::CoordSystem()
{
    count = 0;
    count_of_real_points = 0;
    table = new QTableWidget(count, 2);
    table->setFixedWidth(width()/2);
    table->setHorizontalHeaderLabels(QStringList() <<"X" <<"Y");
    draw_points = new QPushButton("Draw Only Points");
    draw_lines = new QPushButton("Draw Lines");
    add_point = new QPushButton("Add Point");
    clear_the_last_point = new QPushButton("Clear Last Point");
    clear = new QPushButton("Clear");

    set_layout();
    set_connections();
    set_style();
}

CoordSystem::~CoordSystem()
{
    delete table;
    delete add_point;
    delete clear_the_last_point;
    delete draw_points;
    delete draw_lines;
    delete clear;
    delete []points;
}

void CoordSystem::set_connections()
{
    connect(draw_points, SIGNAL(clicked(bool)),
            this, SLOT(repaint()));
    connect(draw_lines, SIGNAL(clicked(bool)),
            this, SLOT(repaint()));
    connect(clear, SIGNAL(clicked(bool)),
            this, SLOT(cleartable()));
    connect(clear, SIGNAL(clicked(bool)),
            this, SLOT(repaint()));
    connect(table, SIGNAL(itemSelectionChanged()),
            this, SLOT(repaint()));
    connect(add_point, SIGNAL(clicked(bool)),
            this, SLOT(add_new_point()));
    connect(clear_the_last_point, SIGNAL(clicked(bool)),
            this, SLOT(clean_last_point()));
}

void CoordSystem::set_layout()
{
    QHBoxLayout* h_draw_layout = new QHBoxLayout;
    h_draw_layout->addWidget(draw_points);
    h_draw_layout->addWidget(draw_lines);

    QHBoxLayout* h_add_clear_layout = new QHBoxLayout;
    h_add_clear_layout->addWidget(add_point);
    h_add_clear_layout->addWidget(clear_the_last_point);

    QVBoxLayout* v_layout = new QVBoxLayout;
    v_layout->addWidget(table);
    v_layout->addLayout(h_draw_layout);
    v_layout->addLayout(h_add_clear_layout);
    v_layout->addWidget(clear);

    QHBoxLayout* h_layout = new QHBoxLayout;
    h_layout->addLayout(v_layout);
    h_layout->addStretch();

    window()->setLayout(h_layout);
    window()->setFixedSize(800, 468);
}

void CoordSystem::set_style()
{

    QPixmap pix(":/pics/img/pic3.jpg");
    pix = pix.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, pix);
    this->setPalette(palette);

    draw_points->setStyleSheet("background-color: yellow");
    draw_lines->setStyleSheet("background-color: yellow");
    add_point->setStyleSheet("background-color: orange");
    clear_the_last_point->setStyleSheet("background-color: orange");
    clear->setStyleSheet("background-color: coral");
}

void CoordSystem::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    count_of_real_points = 0;

    QPainter painter(this);
    draw_axis(&painter);
    make_array_of_points();

    draw_points_func(&painter);
    button_setup();

    QPushButton* senderbutton = qobject_cast<QPushButton*>(sender());
    if(senderbutton == draw_lines)
    {
        draw_lines_func(&painter);
    }
}

void CoordSystem::mousePressEvent(QMouseEvent* event)
{
    QPoint m_pos = event->pos();
    for(int i = 0; i < count; i++)
    {
        if(is_empty_row(i))
        {
            QTableWidgetItem* x_coord = new QTableWidgetItem(QString::number(m_pos.rx() - x0));
            QTableWidgetItem* y_coord = new QTableWidgetItem(QString::number(y0 - m_pos.ry()));
            table->setItem(i, 0, x_coord);
            table->setItem(i, 1, y_coord);
            repaint();
            return;
        }
    }
    table->insertRow(table->rowCount());
    count++;
    QTableWidgetItem* x_coord = new QTableWidgetItem(QString::number(m_pos.rx() - x0));
    QTableWidgetItem* y_coord = new QTableWidgetItem(QString::number(y0 - m_pos.ry()));
    table->setItem(table->rowCount() - 1, 0, x_coord);
    table->setItem(table->rowCount() - 1, 1, y_coord);
    repaint();
}

void CoordSystem::draw_axis(QPainter* painter)
{
    int x0 = 560;
    int y0 = 234;

    QPen pen;
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawLine(x0, 0, x0, 468);
    painter->drawLine(table->width(), y0, 800, y0);


    painter->drawText(x0 - 4, 10 ,"^ Y");
    painter->drawText(width() - 10, y0 + 4, ">");
    painter->drawText(width() - 10, y0 + 15 ,"X");
    painter->drawText(x0 + 2, y0 + 10, "0");

    pen.setWidth(1);
    painter->setPen(pen);
    for(int i = x0; i < width() - 10; i += 10)
    {
        painter->drawLine(i, y0 - 3, i, y0 + 3);
        if((i - x0) % 50 == 0 && i - x0 != 0)
        {
            painter->drawText(i - 5, y0 + 15 , QString::number(i - x0));
        }
    }
    for(int i = x0; i > table->width(); i -= 10)
    {
        painter->drawLine(i, y0 - 3, i, y0 + 3);
        if((x0 - i) % 50 == 0 && x0 - i != 0)
        {
            painter->drawText(i - 8, y0 + 15 , QString::number(i - x0));
        }
    }
    for(int i = y0; i > 10; i -= 10)
    {
        painter->drawLine(x0 - 3, i, x0 + 3, i);
        if((y0 - i) % 50 == 0 && y0 - i != 0)
        {
            painter->drawText(x0 + 8, i + 5, QString::number(y0 - i));
        }
    }
    for(int i = y0; i < height(); i += 10)
    {
        painter->drawLine(x0 - 3, i, x0 + 3, i);
        if((i - y0) % 50 == 0 && i - y0 != 0)
        {
            painter->drawText(x0 + 8, i + 5, QString::number(y0 - i));
        }
    }
}

void CoordSystem::draw_points_func(QPainter* painter)
{
    QPen pen;
    painter->setPen(pen);

    for(int i = 0; i < count_of_real_points; i++)
    {
        painter->setBrush(QColor((rand() + points[i].rx()) % 256,
                                 (rand() + points[i].ry()) % 256,
                                  rand() % 256));
        painter->drawEllipse(points[i], 5, 5);
    }
}

void CoordSystem::draw_lines_func(QPainter* painter)
{
    sort_by_x();
    sort_by_y();

    QPen pen;
    pen.setWidth(2);

    for(int i = 0; i < count_of_real_points; i += 2)
    {
        pen.setColor(QColor((rand() + points[i].rx()) % 256,
                                 (rand() + points[i].ry()) % 256,
                                  rand() % 256));
        painter->setPen(pen);
        painter->drawLine(points[i], points[i + 1]);
    }
}

void CoordSystem::make_array_of_points()
{
    points = new QPoint[table->rowCount()];
    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem* current_x = table->item(i, 0);
        QTableWidgetItem* current_y = table->item(i, 1);

        if(!is_full_row(i))
        {
            continue;
        }

        points[count_of_real_points].rx() = x0 + current_x->text().toInt();
        points[count_of_real_points].ry() = y0 - current_y->text().toInt();
        count_of_real_points++;
    }
}

void CoordSystem::sort_by_x()
{
    for(int i = 0; i < count_of_real_points; i++)
    {
        for(int j = i + 1; j < count_of_real_points; j++)
        {
            if(points[i].rx() > points[j].rx())
            {
                int helpx = points[i].rx();
                points[i].rx() = points[j].rx();
                points[j].rx() = helpx;
                int helpy = points[i].ry();
                points[i].ry() = points[j].ry();
                points[j].ry() = helpy;
            }
        }
    }
}

void CoordSystem::sort_by_y()
{
    for(int i = 0; i < count_of_real_points; i++)
    {
        int count1 = 0;
        for(int j = i; points[i].rx() == points[j].rx(); j++)
        {
            count1++;
        }
        if(count1 > 1)
        {
            for(int j = i; j < i + count1; j++)
            {
                for(int k = j + 1; k < i + count1; k++)
                {
                    if(points[j].ry() < points[k].ry())
                    {
                        int helpy = points[j].ry();
                        points[j].ry() = points[k].ry();
                        points[k].ry() = helpy;
                    }
                }
            }
            i = i + count1 - 1;
        }
    }
}

void CoordSystem::button_setup() const
{
    if(points_are_even())
    {
        draw_lines->setDisabled(true);
    }
    else
    {
        draw_lines->setDisabled(false);
    }
}

bool CoordSystem::points_are_even() const
{
    return count_of_real_points % 2;
}

bool CoordSystem::is_full_row(int row) const
{
    QTableWidgetItem* current_x = table->item(row,0);
    QTableWidgetItem* current_y = table->item(row,1);

    if (!current_x || current_x->text() == "") {
      return false;
    }
    if (!current_y || current_y->text() == "") {
      return false;
    }
    return true;
}

bool CoordSystem::is_empty_row(int row) const
{
    QTableWidgetItem* current_x = table->item(row,0);
    QTableWidgetItem* current_y = table->item(row,1);
    if ((!current_x || current_x->text() == "") && (!current_y || current_y->text() == "")) {
      return true;
    }
    return false;
}

void CoordSystem::add_new_point()
{
    table->insertRow(table->rowCount());
    count++;
    repaint();
}

void CoordSystem::clean_last_point()
{
    if(count == 0)
    {
        QMessageBox* msg = new QMessageBox;
        msg->setInformativeText("Table is empty!");
        msg->show();
        return;
    }
    table->removeRow(table->rowCount() - 1);
    count--;
    repaint();
}

void CoordSystem::cleartable()
{
    table->setRowCount(0);
    count = 0;
    repaint();
}
