#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"

extern int agentBodyType;

/*
 * Real game beaming.
 * Filling params x y angle
 */
void NaoBehavior::beam(double &beamX, double &beamY, double &beamAngle)
{
    //beamX = -HALF_FIELD_X + worldModel->getUNum();

    switch (worldModel->getUNum())
    {
    case 1:
        beamX = -HALF_FIELD_X + 1;
        beamY = 0;
        beamAngle = 0;
        break;
    case 2:
        beamX = -HALF_FIELD_X + 2;
        beamY = 0;
        beamAngle = 0;
        break;
    case 4:
        beamX = -HALF_FIELD_X + 3.5;
        beamY = 0;
        beamAngle = 0;
        break;
    case 3:
        beamX = -HALF_FIELD_X + 8.5;
        beamY = 0;
        beamAngle = 0;
        break;
    case 5:
        beamX = -0.5;
        beamY = 3.5;
        beamAngle = 0;
        break;
    case 6:
        beamX = -0.5;
        beamY = -3.5;
        beamAngle = 0;
        break;
    case 7:
        beamX = -3;
        beamY = 2;
        beamAngle = 0;
        break;
    case 8:
        beamX = -3;
        beamY = -2;
        beamAngle = 0;
        break;
    case 9:
        beamX = -0.5;
        beamY = 2.3;
        beamAngle = 0;
        break;
    case 11:
        beamX = -0.5;
        beamY = -2.3;
        beamAngle = 0;
        break;
    case 10:
        beamX = -1;
        beamY = 0;
        beamAngle = 0;
        break;
    }

    //cout << "*********************************\n";
    //cout << worldModel->getUNum() << "\n";
    //beamY = 3;
    //beamAngle = 0;
}

VecPosition NaoBehavior::getGKLocation()
{
    VecPosition gate = VecPosition(-HALF_FIELD_X, 0, 0);
    VecPosition mypos = worldModel->getMyPosition();
    double x1 = ball.getX();
    double y1 = ball.getY();
    double xg = gate.getX();
    double x2 = mypos.getX();
    double y2 = mypos.getY();
    double k1 = y1 / (x1 - xg);
    double b1 = -1 * y1 * xg / (x1 - xg);
    double k2 = -1 * (x1 - xg) / y1;
    double b2 = y2 + (x1 - xg) / y1 * x2;
    return VecPosition((b2 - b1) / (k1 - k2), k1 * (b2 - b1) / (k1 - k2) + b1, 0);
}


//返回离球最近的球员编号
int NaoBehavior::getClosestMember()
{
    int playerClosestToBall = -1;
    double closestDistanceToBall = 10000;
    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i)
    {
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum)
        {
            // This is us
            temp = worldModel->getMyPosition();
        }
        else
        {
            WorldObject *teammate = worldModel->getWorldObject(i);
            if (teammate->validPosition)
            {
                temp = teammate->pos;
            }
            else
            {
                continue;
            }
        }
        double distanceToBall = temp.getDistanceTo(ball);
        if (distanceToBall < closestDistanceToBall)
        {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
    }
    return playerClosestToBall;
}

//判断哪个人拿球，返回球员号，-1表示没人拿球
int NaoBehavior::getQiuYuan()
{
    int playerClosestToBall = -1;
    double closestDistanceToBall = 10000;
    int playerNum=1;
    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i)
    {
        VecPosition temp;
        playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum)
        {
            // This is us
            temp = worldModel->getMyPosition();
        }
        else
        {
            WorldObject *teammate = worldModel->getWorldObject(i);
            if (teammate->validPosition)
            {
                temp = teammate->pos;
            }
            else
            {
                continue;
            }
        }
        temp.setZ(0);
		
		double distanceToBall = temp.getDistanceTo(ball);
        if (distanceToBall < closestDistanceToBall)
        {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
	}
        double op_closestDistanceToBall = 999;
        for (int i = WO_OPPONENT1; i < WO_OPPONENT1 + NUM_AGENTS; ++i)
        {
            VecPosition temp;
            WorldObject *ops = worldModel->getWorldObject(i);
            if (ops->validPosition)
            {
                temp = ops->pos;
            }
            else
            {
                continue;
            }
            double op_distanceToBall = temp.getDistanceTo(ball);
            if (op_distanceToBall < op_closestDistanceToBall)
            {
                op_closestDistanceToBall = op_distanceToBall;
            }
        }

        if (closestDistanceToBall < 0.25 || op_closestDistanceToBall > closestDistanceToBall)
        {
		    cout << "QiuYuanNUM: " << playerClosestToBall << "******************\n";
            return playerClosestToBall;
        }
        else
        {
            cout << "Not us " << "******************\n";
            return -1;
        }
    
}

//看角度上下5度范围内是对手的数量
int NaoBehavior::isOPHere(double angle, double r)
{
    double jiaodu;

    VecPosition myPosition = worldModel->getMyPosition();

    for (int i = WO_OPPONENT1; i < WO_OPPONENT1 + NUM_AGENTS; ++i)
    {
        VecPosition temp;
        int playerNum = i - WO_OPPONENT1 + 1;

        WorldObject *opponent = worldModel->getWorldObject(i);
        if (opponent->validPosition)
        {
            temp = opponent->pos;
        }
        else
        {
            continue;
        }
        temp.setZ(0);

        jiaodu = atan2Deg(temp.getX() - myPosition.getX(), -temp.getY() + myPosition.getY());

        double cha = jiaodu - angle;

        if (cha > -5 && cha < 5 && myPosition.getDistanceTo(temp) < r)
        {
            return 1;
        }
    }
    return 0;
}

//看角度上下2度范围内是对手的数量
int NaoBehavior::isOPHere1(double angle, double r)
{
    double jiaodu;

    VecPosition myPosition = worldModel->getMyPosition();

    for (int i = WO_OPPONENT1; i < WO_OPPONENT1 + NUM_AGENTS; ++i)
    {
        VecPosition temp;
        int playerNum = i - WO_OPPONENT1 + 1;

        WorldObject *opponent = worldModel->getWorldObject(i);
        if (opponent->validPosition)
        {
            temp = opponent->pos;
        }
        else
        {
            continue;
        }
        temp.setZ(0);

        jiaodu = atan2Deg(temp.getX() - myPosition.getX(), -temp.getY() + myPosition.getY());

        double cha = jiaodu - angle;

        if (cha > -2 && cha < 2 && myPosition.getDistanceTo(temp) < r)
        {
            return 1;
        }
    }
    return 0;
}

//向对面射门时避开对面的守门员，返回射门的位置(VecPosition)
VecPosition NaoBehavior::avoidGK()
{
    VecPosition mypos = worldModel->getMyPosition();
    double jiaodu = atan2Deg(HALF_FIELD_X - mypos.getX(), mypos.getY());

    if (isPeopleHere1(jiaodu, 4.5) == 0)
    {
        return VecPosition(HALF_FIELD_X, 0, 0);
    }

    if (mypos.getY() > 0)
    {
        //在上半场时朝球门中心0.5向上判断
        if (isPeopleHere1(atan2Deg(HALF_FIELD_X - mypos.getX(), mypos.getY() - 0.5), 4.5) == 0)
        {
            return VecPosition(HALF_FIELD_X ,0.5, 0);
        }
        else if (isPeopleHere1(atan2Deg(HALF_FIELD_X - mypos.getX(), mypos.getY() - 1), 4.5) == 0)
        {
            return VecPosition(HALF_FIELD_X,1, 0);
        }
    }
    else if (mypos.getY() <= 0)
    {
        //在下半场时朝球门中心0.5向下判断
        if (isPeopleHere1(atan2Deg(HALF_FIELD_X - mypos.getX(), mypos.getY() + 0.5), 4.5) == 0)
        {
            return VecPosition(HALF_FIELD_X, -0.5, 0);
        }
        else if (isPeopleHere1(atan2Deg(HALF_FIELD_X - mypos.getX(), mypos.getY() + 1), 4.5) == 0)
        {
            return VecPosition(HALF_FIELD_X, -1, 0);
        }
    }
    //角度上都有人就直射
    return VecPosition(HALF_FIELD_X, 0, 0);
}

//看角度上下5度范围内队友的数量
int NaoBehavior::isTMHere(double angle, double r)
{
    double jiaodu;

    int count = 0;

    VecPosition myPosition = worldModel->getMyPosition();

    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i)
    {
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;

        WorldObject *teammate = worldModel->getWorldObject(i);
        if (teammate->validPosition)
        {
            temp = teammate->pos;
        }
        else
        {
            continue;
        }
        temp.setZ(0);

        jiaodu = atan2Deg(temp.getX() - myPosition.getX(), -temp.getY() + myPosition.getY());

        double cha = jiaodu - angle;

        if (cha > -5 && cha < 5 && myPosition.getDistanceTo(temp) < r)
        {
            count++;
        }
    }
    return count;
}

int NaoBehavior::isPeopleHere(double angle, double r)
{
    return isOPHere(angle, r) + isTMHere(angle, r);
}

//看角度上下5度范围内队友的数量
int NaoBehavior::isTMHere1(double angle, double r)
{
    double jiaodu;

    int count = 0;

    VecPosition myPosition = worldModel->getMyPosition();

    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i)
    {
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;

        WorldObject *teammate = worldModel->getWorldObject(i);
        if (teammate->validPosition)
        {
            temp = teammate->pos;
        }
        else
        {
            continue;
        }
        temp.setZ(0);

        jiaodu = atan2Deg(temp.getX() - myPosition.getX(), -temp.getY() + myPosition.getY());

        double cha = jiaodu - angle;

        if (cha > -2 && cha < 2 && myPosition.getDistanceTo(temp) < r)
        {
            count++;
        }
    }
    return count;
}

int NaoBehavior::isPeopleHere1(double angle, double r)
{
    return isOPHere1(angle, r) + isTMHere1(angle, r);
}

VecPosition NaoBehavior::getSafePlace()
{
    VecPosition mypos = worldModel->getMyPosition();
    if (isPeopleHere(90, 3) == 0)
        return mypos + VecPosition(3, 0, 0);
    else if (isPeopleHere(70, 3) == 0)
        return mypos + VecPosition(3 * sin(70), -3 * cos(70), 0);
    else if (isPeopleHere(110, 3) == 0)
        return mypos + VecPosition(3 * sin(110), -3 * cos(110), 0);
    else if (isPeopleHere(50, 3) == 0)
        return mypos + VecPosition(3 * sin(50), -3 * cos(50), 0);
    else if (isPeopleHere(130, 3) == 0)
        return mypos + VecPosition(3 * sin(130), -3 * cos(130), 0);
    else if (isPeopleHere(30, 3) == 0)
        return mypos + VecPosition(3 * sin(30), -3 * cos(30), 0);
    else if (isPeopleHere(150, 3) == 0)
        return mypos + VecPosition(3 * sin(150), -3 * cos(150), 0);
    else if (isPeopleHere(10, 3) == 0)
        return mypos + VecPosition(3 * sin(10), -3 * cos(10), 0);
    else if (isPeopleHere(170, 3) == 0)
        return mypos + VecPosition(3 * sin(170), -3 * cos(170), 0);
    return VecPosition(-100, 0, 0);
}

int NaoBehavior::isFrontHere()
{
    int sign1 = 0, sign2 = 0;
    VecPosition myPosition = worldModel->getMyPosition();

    for (int i = WO_OPPONENT1; i < WO_OPPONENT1 + NUM_AGENTS; ++i)
    {
        VecPosition temp;
        int playerNum = i - WO_OPPONENT1 + 1;

        WorldObject *teammate = worldModel->getWorldObject(i);
        if (teammate->validPosition)
        {
            temp = teammate->pos;
        }
        else
        {
            continue;
        }
        temp.setZ(0);

        if (myPosition.getX() < temp.getX())
            sign1 = 1;
        if (myPosition.getDistanceTo(temp) > 4)
            sign2 = 1;
    }
    if (sign1 == 0 || sign2 == 0)
    {
        return 1;
    }
    return 0;
}

SkillType NaoBehavior::kickOverMan(VecPosition dest)
{
    VecPosition mypos = worldModel->getMyPosition();
    if (mypos.getDistanceTo(VecPosition(HALF_FIELD_X, 0, 0)) < 4.5)
        return kickBall(KICK_IK, avoidGK());
    //dest=collisionAvoidance(true /*teammate*/, true /*opponent*/, false/*ball*/, 2 /*proximity thresh*/, .5 /*collision thresh*/, dest, true /*keepDistance*/);
    return kickBall(KICK_IK, dest);
/*
    if (isPeopleHere(90, 3) == 0)
        return kickBall(KICK_DRIBBLE, dest);
    else if (isPeopleHere(60, 3) == 0)
        return kickBall(KICK_DRIBBLE, ball + VecPosition(4.3301, -2.5, 0));
    else if (isPeopleHere(120, 3) == 0)
        return kickBall(KICK_DRIBBLE, ball + VecPosition(4.3301, 2.5, 0));
    else if (isPeopleHere(30, 3) == 0)
        return kickBall(KICK_DRIBBLE, ball + VecPosition(2.5, -4.3301, 0));
    else if (isPeopleHere(150, 3) == 0)
        return kickBall(KICK_DRIBBLE, ball + VecPosition(2.5, 4.3301, 0));
    else if (isPeopleHere(0, 3) == 0)
        return kickBall(KICK_DRIBBLE, ball + VecPosition(0, -5, 0));
    else if (isPeopleHere(180, 3) == 0)
        return kickBall(KICK_DRIBBLE, ball + VecPosition(0, 5, 0));
    else
        return kickBall(KICK_DRIBBLE, dest);
*/
}

SkillType NaoBehavior::kickBallDire()
{
    VecPosition dire = ball - worldModel->getMyPosition();
    //if(atan2Deg(dire.getY(),dire.getX())<180)
    if (ball.getX() > 3.5)
    {
        if (ball.getY() > 0.5)
        {
            return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, -0.5, 0));
        }
        else if (ball.getY() < -0.5)
        {
            return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0.5, 0));
        }
        else
            return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0));
    }
    else
    {
        if (dire.getX() < 0)
        {
            if (dire.getY() > 0)
            {
                return kickBall(KICK_IK, ball + dire + VecPosition(100, 100, 0));
            }
            else
            {
                return kickBall(KICK_IK, ball + dire + VecPosition(100, -100, 0));
            }
        }
        else
        {
            return kickBall(KICK_IK, (ball + dire) * 1000);
        }
    }

    //else
    //{
    //    return goToTarget(ball+VecPosition(-1,0,0));
    //}
}

VecPosition NaoBehavior::jieqiu()
{
    VecPosition mypos = worldModel->getMyPosition();
    //VecPosition ball = worldModel->getBall();
    //VecPosition tm = theCloseTM();
    VecPosition op = theCloseOP();
    double k1 = (op.getY() - mypos.getY()) / (op.getX() - mypos.getX());
    double k2 = (op.getY() - ball.getY()) / (op.getX() - ball.getX());
    VecPosition halfpoint = VecPosition((mypos.getX() + op.getX()) / 2, (mypos.getY() + op.getY()) / 2);
    double k3 = (-1) / k1;
    double fenmu = k2 - k3;
    double x = (k2 * ball.getX() - k3 * halfpoint.getX() + halfpoint.getY() - ball.getY()) / fenmu;
    double y = k3 * (x - halfpoint.getX()) + halfpoint.getY();
    return VecPosition(x, y, 0);
}

VecPosition NaoBehavior::theCloseOP()
{
    double distance = 9999;
    VecPosition player;
    for (int i = WO_OPPONENT1; i < WO_OPPONENT1 + NUM_AGENTS; i++)
    {
        VecPosition temp;
        WorldObject *opponent = worldModel->getWorldObject(i);
        if (opponent->validPosition)
        {
            temp = opponent->pos;
        }
        else
        {
            continue;
        }
        if (temp.getDistanceTo(ball) < distance)
        {
            distance = temp.getDistanceTo(ball);
            player = temp;
        }
    }
    return player;
}
VecPosition NaoBehavior::theCloseTM()
{
    double distance = 9999;
    VecPosition player;
    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i)
    {
        VecPosition temp;
        WorldObject *teammate = worldModel->getWorldObject(i);
        if (teammate->validPosition)
        {
            temp = teammate->pos;
        }
        else
        {
            continue;
        }
        if (temp.getDistanceTo(ball) < distance)
        {
            distance = temp.getDistanceTo(ball);
            player = temp;
        }
    }
    return player;
}

SkillType NaoBehavior::selectSkill()
{
    //||worldModel->getPlayMode()==PM_KICK_OFF_RIGHT
    int startplayer = getClosestMember();
    VecPosition mypos = worldModel->getMyPosition();
    if (worldModel->getPlayMode() == PM_KICK_OFF_LEFT || worldModel->getPlayMode() == PM_KICK_OFF_RIGHT || worldModel->getPlayMode() == PM_FREE_KICK_LEFT || worldModel->getPlayMode() == PM_FREE_KICK_RIGHT)
    {
        if (worldModel->getUNum() == startplayer)
        {
            if (mypos.getY() >= 0)
            {
                return kickBall(KICK_FORWARD, VecPosition(8.6603, -8, 2));
                //return kickBall(KICK_DRIBBLE,VecPosition(8.6603,-5,2));
            }
            else
            {
                return kickBall(KICK_FORWARD, VecPosition(8.6603, 8, 2));
                //return kickBall(KICK_DRIBBLE,VecPosition(8.6603,5,2));
            }
            //return kickBall(KICK_FORWARD,VecPosition(8.6603,-5,5));
            //return kickBall(KICK_FORWARD,VecPosition(HALF_FIELD_X,0,5));
        }
        else
        {
            return SKILL_STAND;
        }
    }
    else if (worldModel->getPlayMode() == PM_CORNER_KICK_LEFT || worldModel->getPlayMode() == PM_CORNER_KICK_RIGHT)
    {
        if (worldModel->getUNum() == startplayer)
        {
            if(ball.getX()<0)
                return kickBall(KICK_FORWARD,VecPosition(0,0,2));
            else{
                return kickBall(KICK_FORWARD,VecPosition(HALF_FIELD_X-3.5,0,2));
            }
        }
        
    }
    else if (worldModel->getPlayMode() == PM_KICK_IN_LEFT || worldModel->getPlayMode() == PM_KICK_IN_RIGHT){
        if (worldModel->getUNum() == startplayer)
        {
            
            return kickBall(KICK_FORWARD,VecPosition(HALF_FIELD_X,0,2));
            
        }else{
            if(worldModel->getMyPosition().getDistanceTo(ball)<5)
                return goToTarget(VecPosition(HALF_FIELD_X,0,2));
        }

    }

    //if(worldModel->getUNum()!=3)
    //    return SKILL_STAND;
    //else
    //    return goToTarget(ball);

    // My position and angle
    //cout << worldModel->getUNum() << ": " << worldModel->getMyPosition() << ",\t" << worldModel->getMyAngDeg() << "\n";

    // Position of the ball
    //cout << worldModel->getBall() << "\n";

    // Example usage of the roboviz drawing system and RVSender in rvdraw.cc.
    // Agents draw the position of where they think the ball is
    // Also see example in naobahevior.cc for drawing agent position and
    // orientation.
    /*
    worldModel->getRVSender()->clear(); // erases drawings from previous cycle
    worldModel->getRVSender()->drawPoint("ball", ball.getX(), ball.getY(), 10.0f, RVSender::MAGENTA);
    */

    // ### Demo Behaviors ###

    // Walk in different directions
    //return goToTargetRelative(VecPosition(1,0,0), 0); // Forward
    //return goToTargetRelative(VecPosition(-1,0,0), 0); // Backward
    //return goToTargetRelative(VecPosition(0,1,0), 0); // Left
    //return goToTargetRelative(VecPosition(0,-1,0), 0); // Right
    //return goToTargetRelative(VecPosition(1,1,0), 0); // Diagonal
    //return goToTargetRelative(VecPosition(0,1,0), 90); // Turn counter-clockwise
    //return goToTargetRelative(VecPdosition(0,-1,0), -90); // Turn clockwise
    //return goToTargetRelative(VecPosition(1,0,0), 15); // Circle

    // Walk to the ball
    //return goToTarget(ball);

    // Turn in place to face ball
    /*
    double distance, angle;
    getTargetDistanceAndAngle(ball, distance, angle);
    if (abs(angle) > 10|| distance > 1) {
      return goToTargetRelative(worldModel->g2l(ball), angle);
    } else {
      return kickBall(KICK_FORWARD, VecPosition(HALF_FIELD_X, 0, 0));  
      //return SKILL_STAND;
    }
*/

    // Find closest player to ball
/*
    if (worldModel->getMyPosition().getDistanceTo(VecPosition(HALF_FIELD_X, 0, 0)) < 4.5)
        return kickBall(KICK_IK, avoidGK());
*/
    if (worldModel->getMyPosition().getDistanceTo(VecPosition(HALF_FIELD_X, 0, 0)) < 4.5)
    {
        int qy = getQiuYuan();

        WorldObject *teammate = worldModel->getWorldObject(qy + WO_TEAMMATE1 - 1);
	    VecPosition temp;

        temp = teammate->pos;

        if (qy > 0 && worldModel->getUNum()!=qy)
        {
            srand((unsigned)time(NULL));
            if(rand() / double(RAND_MAX)>0.5)
                return goToTarget(temp + VecPosition(-1.5, 1.5, 0));
            else
                return goToTarget(temp + VecPosition(-1.5, -1.5, 0));
        }else{
            return kickBall(KICK_IK, avoidGK());
        }
    }
    int playerClosestToBall1 = -3;
    int playerClosestToBall2 = -2;
    int playerClosestToBall3 = -1;
    int playerClosestToBall4 = -1;
    double closestDistanceToBall1 = 10000;
    double closestDistanceToBall2 = 10000;
    double closestDistanceToBall3 = 10000;
    double closestDistanceToBall4 = 10000;
    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i)
    {
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;

        if (playerNum == 1 || playerNum == 2 || playerNum == 4 || playerNum == 11)
        {
            continue;
        }

        if (worldModel->getUNum() == playerNum)
        {
            // This is us
            temp = worldModel->getMyPosition();
        }
        else
        {
            WorldObject *teammate = worldModel->getWorldObject(i);
            if (teammate->validPosition)
            {
                temp = teammate->pos;
            }
            else
            {
                continue;
            }
        }
        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(ball);
        if (distanceToBall < closestDistanceToBall1)
        {
            playerClosestToBall4 = playerClosestToBall3;
            closestDistanceToBall4 = closestDistanceToBall3;

            playerClosestToBall3 = playerClosestToBall2;
            closestDistanceToBall3 = closestDistanceToBall2;

            playerClosestToBall2 = playerClosestToBall1;
            closestDistanceToBall2 = closestDistanceToBall1;

            playerClosestToBall1 = playerNum;
            closestDistanceToBall1 = distanceToBall;
        }
        else if (distanceToBall < closestDistanceToBall2)
        {
            playerClosestToBall4 = playerClosestToBall3;
            closestDistanceToBall4 = closestDistanceToBall3;

            playerClosestToBall3 = playerClosestToBall2;
            closestDistanceToBall3 = closestDistanceToBall2;

            playerClosestToBall2 = playerNum;
            closestDistanceToBall2 = distanceToBall;
        }
        else if (distanceToBall < closestDistanceToBall3)
        {
            playerClosestToBall4 = playerClosestToBall3;
            closestDistanceToBall4 = closestDistanceToBall3;

            playerClosestToBall3 = playerNum;
            closestDistanceToBall3 = distanceToBall;
        }
        else if (distanceToBall < closestDistanceToBall4)
        {
            playerClosestToBall4 = playerNum;
            closestDistanceToBall4 = distanceToBall;
        }
    }

    double dis = ball.getX();
    double disY = ball.getY();
    double shootY;
    if (disY > 3)
        shootY = 1;
    else if (disY < -3)
        shootY = -1;
    else
        shootY = 0;
    VecPosition target1, target2;
    if (worldModel->getUNum() == 1)
    {
        if (mypos.getDistanceTo(ball) < mypos.getDistanceTo(getGKLocation()) + 2.0)
        {
            //cout << "******X:" << getGKLocation().getX() << "********Y:" << getGKLocation().getY();
            if (mypos.getDistanceTo(getGKLocation()) > 0.5)
                return goToTarget(getGKLocation());
            else
                return kickBall(KICK_IK, getSafePlace());
        }

        else
        {
            target1 = VecPosition(-HALF_FIELD_X + 2, 0.5, 0);
            target2 = VecPosition(-HALF_FIELD_X + 2, -0.5, 0);
            if (dis > 0)
            {
                target1 += VecPosition(dis, 0, 0);
                target2 += VecPosition(dis, 0, 0);
            }
            if (worldModel->getMyPosition().getDistanceTo(target1) < 0.1)
            {
                /* code */
                flag = 1;
            }
            if (worldModel->getMyPosition().getDistanceTo(target2) < 0.1)
            {
                flag = 0;
            }
            if (flag == 0)
            {
                target1 = collisionAvoidance(true /*teammate*/, false /*opponent*/, false /*ball*/, 1 /*proximity thresh*/, .5 /*collision thresh*/, target1, true /*keepDistance*/);
                return goToTarget(target1);
            }
            else
            {
                target2 = collisionAvoidance(true /*teammate*/, false /*opponent*/, false /*ball*/, 1 /*proximity thresh*/, .5 /*collision thresh*/, target2, true /*keepDistance*/);
                return goToTarget(target2);
            }
        }
    }
    else if (worldModel->getUNum() == 2)
    {
        target1 = VecPosition(-HALF_FIELD_X + 4.5, 3, 0);
        target2 = VecPosition(-HALF_FIELD_X + 4.5, 1, 0);
        if (dis > 0)
        {
            target1 += VecPosition(dis, 0, 0);
            target2 += VecPosition(dis, 0, 0);
        }
        if (mypos.getDistanceTo(ball) < 4.5 || ball.getX() < mypos.getX())
            if (getQiuYuan() > 0 && worldModel->getUNum() != getQiuYuan())
                return goToTarget(getGKLocation());
            else
            {
                if (mypos.getDistanceTo(getGKLocation()) > 0.5)
                    return goToTarget(getGKLocation());
                else
                    return kickBall(KICK_IK, getSafePlace());
            }
        if (mypos.getDistanceTo(target1) < 0.1)
        {
            /* code */
            flag = 1;
        }
        if (mypos.getDistanceTo(target2) < 0.1)
        {
            flag = 0;
        }
        if (flag == 0)
        {
            target1 = collisionAvoidance(true /*teammate*/, false /*opponent*/, false /*ball*/, 1 /*proximity thresh*/, .5 /*collision thresh*/, target1, true /*keepDistance*/);
            return goToTarget(target1);
        }
        else
        {
            target2 = collisionAvoidance(true /*teammate*/, false /*opponent*/, false /*ball*/, 1 /*proximity thresh*/, .5 /*collision thresh*/, target2, true /*keepDistance*/);
            return goToTarget(target2);
        }
    }
    else if (worldModel->getUNum() == 4)
    {
        target1 = VecPosition(-HALF_FIELD_X + 4.5, -3, 0);
        target2 = VecPosition(-HALF_FIELD_X + 4.5, -1, 0);
        if (dis > 0)
        {
            target1 += VecPosition(dis, 0, 0);
            target2 += VecPosition(dis, 0, 0);
        }
        if (mypos.getDistanceTo(ball) < 4.5 || ball.getX() < mypos.getX())
            if (getQiuYuan() > 0 && worldModel->getUNum() != getQiuYuan())
                return goToTarget(getGKLocation());
            else
            {
                if (worldModel->getUNum() == getQiuYuan())
                    return kickBall(KICK_IK, getSafePlace());
                if (mypos.getDistanceTo(getGKLocation()) > 0.5)
                    return goToTarget(getGKLocation());
                else
                    return kickBall(KICK_IK, getSafePlace());
            }
        if (mypos.getDistanceTo(target1) < 0.1)
        {
            /* code */
            flag = 1;
        }
        if (mypos.getDistanceTo(target2) < 0.1)
        {
            flag = 0;
        }
        if (flag == 0)
        {
            target1 = collisionAvoidance(true /*teammate*/, true /*opponent*/, false /*ball*/, 1 /*proximity thresh*/, .5 /*collision thresh*/, target1, true /*keepDistance*/);
            return goToTarget(target1);
        }
        else
        {
            target2 = collisionAvoidance(true /*teammate*/, true /*opponent*/, false /*ball*/, 1 /*proximity thresh*/, .5 /*collision thresh*/, target2, true /*keepDistance*/);
            return goToTarget(target2);
        }
    }
    // else if (worldModel->getUNum() == 11)
    // {
    //     target1 = VecPosition(4, 0.5, 0);
    //     target2 = VecPosition(4, -0.5, 0);

    //     target1 += VecPosition(dis / 4, disY / 2, 0);
    //     target2 += VecPosition(dis / 4, disY / 2, 0);

    //     if (mypos.getDistanceTo(target1) < 5)
    //     {
    //         if (getQiuYuan() < 0)
    //             return kickOverMan(VecPosition(HALF_FIELD_X, 0, 0));
    //         else
    //             return goToTarget(ball + VecPosition(2, 1, 0));
    //     }
    //     if (mypos.getDistanceTo(target1) < 0.1)
    //     {
    //         /* code */
    //         flag = 1;
    //     }
    //     if (mypos.getDistanceTo(target2) < 0.1)
    //     {
    //         flag = 0;
    //     }
    //     if (flag == 0)
    //     {
    //         target1 = collisionAvoidance(true /*teammate*/, false /*opponent*/, true /*ball*/, 1 /*proximity thresh*/, .5 /*collision thresh*/, target1, true /*keepDistance*/);
    //         return goToTarget(target1);
    //     }
    //     else
    //     {
    //         target2 = collisionAvoidance(true /*teammate*/, false /*opponent*/, true /*ball*/, 1 /*proximity thresh*/, .5 /*collision thresh*/, target2, true /*keepDistance*/);
    //         return goToTarget(target2);
    //     }
    // }

    int numberOfQY = getQiuYuan();
    int num[7] = {5, 6, 7, 8, 9, 10, 11};
	int anum[8] = {3,5,6,7,8,9,10,11};

    if (numberOfQY == -1)
    {
        if (worldModel->getUNum() == playerClosestToBall1 || worldModel->getUNum() == playerClosestToBall2)
        {
            if (ball.getX() < -5)
                return kickBall(KICK_IK, getSafePlace());
            //VecPosition tm = theCloseTM();
		    
            

            VecPosition op = theCloseOP();
            if (ball.getX() - op.getX() > 0)
            {
                if (mypos.getX() - ball.getX() > 0)
                {
                    VecPosition temp = jieqiu();
                    if (mypos.getDistanceTo(temp) > 0.5)
                    {
                        return goToTarget(temp);
                    }
                    else
                    {
                        return kickOverMan(VecPosition(HALF_FIELD_X, 0, 0));
                    }
                }
            }
            else
            {
                if (mypos.getX() - ball.getX() <= 0)
                {
                    VecPosition temp = jieqiu();
                    if (mypos.getDistanceTo(temp) > 0.5)
                    {
                        return goToTarget(temp);
                    }
                    else
                    {
                        return kickOverMan(VecPosition(HALF_FIELD_X, 0, 0));
                    }
                }
            }
            return kickOverMan(VecPosition(HALF_FIELD_X, 0, 0));
            //return kickBallDire();
        }
        if (worldModel->getUNum() == 3)
        {
            target1 = VecPosition(-HALF_FIELD_X / 2, 1, 0);
            target2 = VecPosition(-HALF_FIELD_X / 2, -1, 0);
            if (dis > 0)
            {
                target1 += VecPosition(dis, 0, 0);
                target2 += VecPosition(dis, 0, 0);
            }
            if (mypos.getDistanceTo(ball) < 4.5 || ball.getX() < mypos.getX())
                if (getQiuYuan() > 0 && worldModel->getUNum() != getQiuYuan())
                    return goToTarget(getGKLocation());
                else
                {
                    return kickBall(KICK_IK, getSafePlace());
                }
            if (mypos.getDistanceTo(target1) < 0.1)
            {
                /* code */
                flag = 1;
            }
            if (mypos.getDistanceTo(target2) < 0.1)
            {
                flag = 0;
            }
            if (flag == 0)
            {
                target1 = collisionAvoidance(true /*teammate*/, false /*opponent*/, true /*ball*/, 1 /*proximity thresh*/, .5 /*collision thresh*/, target1, true /*keepDistance*/);
                return goToTarget(target1);
            }
            else
            {
                target2 = collisionAvoidance(true /*teammate*/, false /*opponent*/, true /*ball*/, 1 /*proximity thresh*/, .5 /*collision thresh*/, target2, true /*keepDistance*/);
                return goToTarget(target2);
            }
        }
        else
        {
            VecPosition targetC[5];
            //与y轴夹角为pi/3，半径3m
            targetC[0] = VecPosition(ball.getX() + 1.299 * 2, ball.getY() - 0.75 * 2, 0);
            //与y轴夹角为2pi/3，半径3m
            targetC[1] = VecPosition(ball.getX() + 1.299 * 2, ball.getY() + 0.75 * 2, 0);
            //与y轴夹角为4pi/3，半径3m
            targetC[2] = VecPosition(ball.getX() - 1.299 * 2, ball.getY() + 0.75 * 2, 0);
            //与y轴夹角为5pi/3，半径3m
            targetC[3] = VecPosition(ball.getX() - 1.299 * 2, ball.getY() - 0.75 * 2, 0);
            
            targetC[4] = VecPosition(ball.getX() - 1.299 * 2 - 1, ball.getY(), 0);
            int j = 0;
            for (int i = 0; i < 7; i++)
            {
                if (num[i] == playerClosestToBall1 || num[i] == playerClosestToBall2)
                {

                    continue;
                }
                if (worldModel->getUNum() == num[i])
                {
                    return goToTarget(targetC[j]);
                }
                j++;
            }
        }
        return kickOverMan(VecPosition(HALF_FIELD_X, 0, 0));
    }
    else
    {
        VecPosition targetC[7];
        //与y轴夹角为pi/5，半径3m
        targetC[0] = VecPosition(ball.getX() + 1.7634 * 1, ball.getY() - 2.4271 * 1, 0);
        //与y轴夹角为2pi/5，半径3m
        targetC[1] = VecPosition(ball.getX() + 2.8532 * 1, ball.getY() - 0.9271 * 1, 0);
        //与y轴夹角为3pi/5，半径3m
        targetC[2] = VecPosition(ball.getX() + 2.8532 * 1, ball.getY() + 0.9271 * 1, 0);
        //与y轴夹角为4pi/5，半径3m
        targetC[3] = VecPosition(ball.getX() + 1.7634 * 1, ball.getY() + 2.4271 * 1, 0);
        //与y轴夹角为-pi/3，半径3m
        targetC[4] = VecPosition(ball.getX() - 1.5 * 1, ball.getY() - 2.5981 * 1, 0);
        //与y轴夹角为-2pi/3，半径3m
        targetC[5] = VecPosition(ball.getX() - 1.5 * 1, ball.getY() + 2.5981 * 1, 0);

        targetC[6] = VecPosition(ball.getX() - 1.5 * 1-1.5, ball.getY(), 0);

        if (worldModel->getUNum() == numberOfQY)
        {
            if (isFrontHere() == 1)
            {
                VecPosition tar = VecPosition(HALF_FIELD_X, 0, 0);
                tar = collisionAvoidance(true, false, true, 1, .5, tar, true);
                return kickOverMan(tar);
                //return kickBall(KICK_IK,VecPosition(HALF_FIELD_X, 0, 0));
            }
            else if (mypos.getDistanceTo(VecPosition(HALF_FIELD_X, 0, 0)) < 4.5)
            {
                return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0));
            }
            else
            {
                //传球给附近队友策略
                /*
                if (isTMHere(36, 3) > 0 && isOPHere(36, 3) == 0)
                {
                    return kickBall(KICK_IK, targetC[0]+VecPosition(1.7634*2,-2.4271*2,0));
                }
                else if (isTMHere(72, 3) > 0 && isOPHere(72, 3) == 0)
                {
                    return kickBall(KICK_IK, targetC[1]+VecPosition(2.8532*2,-0.9271*2,0));
                }
                else if (isTMHere(108, 3) > 0 && isOPHere(108, 3) == 0)
                {
                    return kickBall(KICK_IK, targetC[2]+VecPosition(2.8532*2,0.9271*2,0));
                }
                else if (isTMHere(144, 3) > 0 && isOPHere(144, 3) == 0)
                {
                    return kickBall(KICK_IK, targetC[3]+VecPosition(1.7634*2,2.4271*2,0));
                }
                
                //如果无队友可传
                if(mypos.getDistanceTo(ball)<0.5){
                    return kickOverMan(VecPosition(HALF_FIELD_X, 0, 0));}
                else{
                    VecPosition tar = ball;
                    tar = collisionAvoidance(true, false, true, 1, .5, tar, true);
                }
                */

                return kickOverMan(VecPosition(HALF_FIELD_X, 0, 0));
                //return kickBall(KICK_IK,VecPosition(HALF_FIELD_X, 0, 0));
            }
        }

        int j = 0;
        for (int i = 0; i < 8; i++)
        {
            if (anum[i] == numberOfQY)
            {
                continue;
            }
            if (worldModel->getUNum() == anum[i])
            {
                return goToTarget(targetC[j]);
            }
            j++;
        }
        return kickOverMan(VecPosition(HALF_FIELD_X, 0, 0));
    }
	//return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0));

    /* if ( closestDistanceToBall1 < 0.25 )
         {
              if ( worldModel->getUNum() == playerClosestToBall1 )
                   return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, shootY, 0));
              else if ( worldModel->getUNum() == playerClosestToBall2 || worldModel->getUNum() == playerClosestToBall3 || worldModel->getUNum() == playerClosestToBall4 )
                   return goToTarget(ball + VecPosition(1,0,0));
         }
      
    if (worldModel->getUNum() == playerClosestToBall1 || worldModel->getUNum() == playerClosestToBall2 || worldModel->getUNum() == playerClosestToBall3 || worldModel->getUNum() == playerClosestToBall4) {
        return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, shootY, 0));
    } 
        */

    // Walk to ball while always facing forward
    //return goToTargetRelative(worldModel->g2l(ball), -worldModel->getMyAngDeg());

    // Dribble ball toward opponent's goal
    //return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X, 0, 0));

    // Kick ball toward opponent's goal
    //return kickBall(KICK_FORWARD, VecPosition(HALF_FIELD_X, 0, 0)); // Basic kick
    //return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0)); // IK kick

    // Just stand in place
    //return SKILL_STAND;

    // Demo behavior where players form a rotating circle and kick the ball
    // back and forth
    //return demoKickingCircle();
}

/*
 * Demo behavior where players form a rotating circle and kick the ball
 * back and forth
 */
SkillType NaoBehavior::demoKickingCircle()
{
    // Parameters for circle
    VecPosition center = VecPosition(-HALF_FIELD_X / 2.0, 0, 0);
    double circleRadius = 5.0;
    double rotateRate = 2.5;

    // Find closest player to ball
    int playerClosestToBall = -1;
    double closestDistanceToBall = 10000;
    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i)
    {
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum)
        {
            // This is us
            temp = worldModel->getMyPosition();
        }
        else
        {
            WorldObject *teammate = worldModel->getWorldObject(i);
            if (teammate->validPosition)
            {
                temp = teammate->pos;
            }
            else
            {
                continue;
            }
        }
        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(ball);
        if (distanceToBall < closestDistanceToBall)
        {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
    }

    if (playerClosestToBall == worldModel->getUNum())
    {
        // Have closest player kick the ball toward the center
        return kickBall(KICK_FORWARD, center);
    }
    else
    {
        // Move to circle position around center and face the center
        VecPosition localCenter = worldModel->g2l(center);
        SIM::AngDeg localCenterAngle = atan2Deg(localCenter.getY(), localCenter.getX());

        // Our desired target position on the circle
        // Compute target based on uniform number, rotate rate, and time
        VecPosition target = center + VecPosition(circleRadius, 0, 0).rotateAboutZ(360.0 / (NUM_AGENTS - 1) * (worldModel->getUNum() - (worldModel->getUNum() > playerClosestToBall ? 1 : 0)) + worldModel->getTime() * rotateRate);

        // Adjust target to not be too close to teammates or the ball
        target = collisionAvoidance(true /*teammate*/, false /*opponent*/, true /*ball*/, 1 /*proximity thresh*/, .5 /*collision thresh*/, target, true /*keepDistance*/);

        if (me.getDistanceTo(target) < .25 && abs(localCenterAngle) <= 10)
        {
            // Close enough to desired position and orientation so just stand
            return SKILL_STAND;
        }
        else if (me.getDistanceTo(target) < .5)
        {
            // Close to desired position so start turning to face center
            return goToTargetRelative(worldModel->g2l(target), localCenterAngle);
        }
        else
        {
            // Move toward target location
            return goToTarget(target);
        }
    }
}
