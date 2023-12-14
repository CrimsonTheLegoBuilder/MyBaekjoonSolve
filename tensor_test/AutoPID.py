import matplotlib.pyplot as plt

class Robot: # 가상의 로봇이 라인트레싱을 한다고 해봅시다.
    def __init__(self, error):
        self.current_error = error
        self.last_action = 0

    def take_action(self, action):
        self.current_error += 0.1 * action
        self.last_action = action

class PID:
    err_sum = 0
    old_err = 0

    def pid(self, current, goal, kp, ki, kd):
        err = goal - current
        self.err_sum += err
        delta_err = err - self.old_err
        self.old_err = err

        return kp*err + ki*self.err_sum + kd*delta_err

class Derivative: #미분 클래스
    def __init__(self):
        self.last_x = 0
        self.last_y = 0

    def get_gradient(self, x, y):
        d = (y - self.last_y) / (x - self.last_x)
        self.last_x = x
        self.last_y = y

        return d

class Train:
    kp = 1.0; ki = 0.5; kd = 0.5
    goal = 0
    episode_length = 100
    learning_rate = 0.001

    def __init__(self):
        self.dp = Derivative()
        self.di = Derivative()
        self.dd = Derivative()
        self.step = 0
        self.last_loss = 0

    def abs_mean(self, list):
        sum = 0
        for i in list:
            sum += i*i
        return sum / len(list)

    def loss(self, draw_graph=False):
        robot = Robot(10)
        pid = PID()

        error = []
        for i in range(self.episode_length): # 로봇의 오차를 축적
            error.append(robot.current_error)
            robot.take_action(pid.pid(robot.current_error, self.goal, self.kp, self.ki, self.kd))

        if draw_graph and self.step % 500 == 0:  # draw graph
            plt.close()
            plt.plot(error)
            plt.title("step={}, p={:.3}, i={:.3}, d={:.3}, train_loss={:.3}".format(self.step, self.kp, self.ki, self.kd, self.abs_mean(error)))
            plt.ylim(-10, 10)
            # plt.savefig("graph\\{}.png".format(self.step))

        return self.abs_mean(error)

    def optimize(self):
        self.kp = self.kp - self.learning_rate * self.dp.get_gradient(self.kp, self.loss())
        self.ki = self.ki - self.learning_rate * self.di.get_gradient(self.ki, self.loss())
        self.kd = self.kd - self.learning_rate * self.dd.get_gradient(self.kd, self.loss(draw_graph=True))
        self.last_loss = self.dd.last_y
        print("step={}, kp={}, ki={}, kd={}, loss={}".format(self.step, self.kp, self.ki, self.kd, self.last_loss))
        self.step += 1

if __name__ == '__main__':
    loss = []
    kp = []
    ki = []
    kd = []

    train = Train()

    for j in range(100000):
        train.optimize()
        loss.append(train.last_loss)
        kp.append(train.kp)
        ki.append(train.ki)
        kd.append(train.kd)

    ideal_kp = train.kp
    ideal_ki = train.ki
    ideal_kd = train.kd

    plt.close()
    plt.plot(loss, label="loss")
    plt.plot(kp, label="Kp")
    plt.plot(ki, label="Ki")
    plt.plot(kd, label="Kd")
    plt.legend()
    plt.title("Train")
    plt.savefig("train.png")