#ifndef CUSTOMROTATIONFILTER_H
#define CUSTOMROTATIONFILTER_H

#include <QRotationFilter>
#include <QQuaternion>

class CustomRotationFilter : public QRotationFilter
{
public:
    CustomRotationFilter();

    bool filter(QRotationReading *reading) override;

    int heading() const;

private:
    int _heading = 0;
};

#endif // CUSTOMROTATIONFILTER_H
