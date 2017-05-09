#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QWidget>
#include <QTimer>
#include "list_item.h"

class ListView : public QWidget
{
    Q_OBJECT
    
public:
    ListView(int height=32, QWidget *parent = 0);
    
    void addItems(QList<ListItem*> items);
    void clearItems();
    
    void addSelections(QList<ListItem*> items, bool recordLastSelection=true);
    void clearSelections(bool clearLastSelection=true);
    
    void setTitles(QList<QString> titles, int height);
    void setSortAlgorithm();
    void setColumnWidths(QList<int> widths);
    
public slots:
    void renderAnimation();
    
protected:
    void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *, QEvent *event);
    void wheelEvent(QWheelEvent *event);
    void handleKeyPressEvent(QKeyEvent *keyEvent);
    void handleButtonPressEvent(QMouseEvent *mouseEvent);
    
    void startScroll(int scrollOffset);
    int adjustRenderOffset(int offset);
    
    void pressHome();
    void pressEnd();
    void pressUp();
    void pressDown();
    void pressPageUp();
    void pressPageDown();
    
    void scrollPageUp();
    void scrollPageDown();
    void scrollHome();
    void scrollEnd();
    
    void shiftSelectHome();
    void shiftSelectEnd();
    void shiftSelectUp();
    void shiftSelectDown();
    void shiftSelectPageUp();
    void shiftSelectPageDown();
    
    void selectAll();
    
    void shiftSelect(int selectionStartIndex, int selectionEndIndex);
    
    void selectScrollUp(int scrollOffset);
    void selectScrollDown(int scrollOffset);
    void shiftSelectScrollUp(int scrollOffset);
    void shiftSelectScrollDown(int scrollOffset);
    
    QList<int> calcuateRenderWidths();
    
    QList<ListItem*> *listItems;
    QList<ListItem*> *selectionItems;
    ListItem *lastSelectItem;
    int rowHeight;
    
private:
    QList<int> columnWidths;
    int titleHeight;
    int scrollUnit;
    int scrollStartY;
    int scrollDistance;
    int renderOffset;
    QList<QString> titleNames;
    
    QTimer *renderTimer;
    int animationDuration;
    int animationFrames;
    int renderTicker;
};

#endif
