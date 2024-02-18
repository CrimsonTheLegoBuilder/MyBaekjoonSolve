const int MIN_ID = 1;
const int MAX_ID = 100000;
const int MIN_TEAM = 1;
const int MAX_TEAM = 5;
const int MIN_SCORE = 1;
const int MAX_SCORE = 5;

struct Node
{
    int id;
    int team;
    Node *prev;
    Node *next;
} soldier[MAX_ID + 1];

struct List
{
    Node head;
    Node tail;

    static void link(Node *front, Node *back)
    {
        front->next = back;
        back->prev = front;
    }

    static void erase(Node *node)
    {
        link(node->prev, node->next);
    }

    void initialize()
    {
        link(&head, &tail);
    }

    void insert(Node *node)
    {
        link(tail.prev, node);
        link(node, &tail);
    }

    bool isEmpty()
    {
        return (head.next == &tail);
    }

    void splice(List *list)
    {
        if (list->isEmpty())
            return;
        link(tail.prev, list->head.next);
        link(list->tail.prev, &tail);
        list->initialize();
    }
} soldierGroup[MAX_TEAM + 1][MAX_SCORE + 1];

void init()
{
    for (int i = MIN_TEAM; i <= MAX_TEAM; i++)
        for (int j = MIN_SCORE; j <= MAX_SCORE; j++)
            soldierGroup[i][j].initialize();
}

void hire(int mID, int mTeam, int mScore)
{
    soldier[mID].id = mID;
    soldier[mID].team = mTeam;
    soldierGroup[mTeam][mScore].insert(soldier + mID);
}

void fire(int mID)
{
    List::erase(soldier + mID);
}

void updateSoldier(int mID, int mScore)
{
    List::erase(soldier + mID);
    soldierGroup[soldier[mID].team][mScore].insert(soldier + mID);
}

void updateTeam(int mTeam, int mChangeScore)
{
    if (mChangeScore > 0)
    {
        for (int i = MAX_SCORE - 1; i >= MIN_SCORE; i--)
        {
            int newScore = i + mChangeScore;
            if (newScore > MAX_SCORE)
                newScore = MAX_SCORE;
            soldierGroup[mTeam][newScore].splice(&soldierGroup[mTeam][i]);
        }
    }
    else if (mChangeScore < 0)
    {
        for (int i = MIN_SCORE + 1; i <= MAX_SCORE; i++)
        {
            int newScore = i + mChangeScore;
            if (newScore < MIN_SCORE)
                newScore = MIN_SCORE;
            soldierGroup[mTeam][newScore].splice(&soldierGroup[mTeam][i]);
        }
    }
}

int bestSoldier(int mTeam)
{
    List *maxScoreGroup;
    for (int i = MAX_SCORE; i >= MIN_SCORE; i--)
    {
        if (!soldierGroup[mTeam][i].isEmpty())
        {
            maxScoreGroup = &soldierGroup[mTeam][i];
            break;
        }
    }

    int maxId = MIN_ID - 1;
    Node *maxScoreSoldier = maxScoreGroup->head.next;
    while (maxScoreSoldier != &(maxScoreGroup->tail))
    {
        if (maxId < maxScoreSoldier->id)
            maxId = maxScoreSoldier->id;
        maxScoreSoldier = maxScoreSoldier->next;
    }
    return maxId;
}