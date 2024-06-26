#include "CustomRotationFilter.h"

CustomRotationFilter::CustomRotationFilter() {}

bool CustomRotationFilter::filter(QRotationReading *reading) {
    // Quaternion components from the sensor reading
    float q1 = reading->x();
    float q2 = reading->y();
    float q3 = reading->z();

    QQuaternion quaternion = QQuaternion::fromEulerAngles(q1, q2, q3);

    // Convert quaternion to Euler angles
    float yaw = qAtan2(2.0f * (quaternion.scalar() * quaternion.y() + quaternion.z() * quaternion.x()),
                       1.0f - 2.0f * (quaternion.y() * quaternion.y() + quaternion.z() * quaternion.z()));
    yaw = qRadiansToDegrees(yaw);
    if (yaw < 0) {
        yaw += 360.0f;
    }

    // Output the heading
    qDebug() << "Heading: " << yaw;

    _heading = yaw;

    // Return true to indicate that the reading should be used
    return true;
}

int CustomRotationFilter::heading() const
{
    return _heading;
}
