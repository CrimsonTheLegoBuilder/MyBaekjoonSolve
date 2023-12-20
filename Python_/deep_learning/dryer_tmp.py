# import tensorflow as tf


class PIDControl:

    def __init__(self, _dt, _min, _max, _kp, _ki, _kd):
        self.update(_dt, _min, _max, _kp, _ki, _kd)

        self.pre_error = 0
        self.integral = 0

    def update(self, _dt, _min, _max, _kp, _ki, _kd):
        self.dt = _dt
        self.min = _min
        self.max = _max
        self.kp = _kp
        self.ki = _ki
        self.kd = _kd

    def calc(self, sv, pv):
        error = sv - pv

        # proportional
        kp = self.kp * error

        # integral
        self.integral += error * self.dt
        ki = self.ki * self.integral

        # differential
        kd = (error - self.pre_error) / self.dt
        kd = self.kd * kd

        # p + i + d
        result = kp + ki + kd

        if result > self.max:
            result = self.max
        elif result < self.min:
            result = self.min

        self.pre_error = error

        # description
        return result

