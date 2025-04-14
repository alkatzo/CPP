#include <iostream>
#include <unordered_map>
#include <list>
#include <memory>
#include <vector>
#include <algorithm>
#include <ranges>
#include <numeric>

class Activity {
public:

    using IdType = int;

    enum class ActivityType {
        COORDINATE,
        TEST,
        RUN_TASK,
        VALIDATE_INPUT
    };

    Activity(int id, const std::string& name, ActivityType type, int size) : id(id), name(name), type(type), size(size) {
    }

    Activity(Activity&& a) = default;
    Activity(const Activity& a) = default;

    Activity& operator=(Activity&& a) = default;
    Activity& operator=(const Activity& a) = default;


    int getId() const
    {
        return id;
    }

    void setId(int newId)
    {
        id = newId;
    }

    std::string getName() const
    {
        return name;
    }

    void setName(const std::string &newName)
    {
        name = newName;
    }

    int getSize() const
    {
        return size;
    }

    void setSize(int newSize)
    {
        size = newSize;
    }

    bool getIsCompleted() const
    {
        return isCompleted;
    }

    void setIsCompleted(bool newIsCompleted)
    {
        isCompleted = newIsCompleted;
    }

    ActivityType getActivityType() const
    {
        return type;
    }

    void setActivityType(ActivityType newType)
    {
        type = newType;
    }


private:
    IdType id;
    std::string name;
    ActivityType type;
    int size;
    bool isCompleted = false;
};

class Worker {
public:
    using IdType = int;

    Worker(int id, const std::string& name, int capacity) : id(id), name(name), capacity(capacity) {

    }

    int getId() const
    {
        return id;
    }

    void setId(int newId)
    {
        id = newId;
    }

    std::string getName() const
    {
        return name;
    }

    void setName(const std::string &newName)
    {
        name = newName;
    }

    int getCapacity() const
    {
        return capacity;
    }

    void setCapacity(int newCapacity)
    {
        capacity = newCapacity;
    }


private:
    IdType id;
    std::string name;
    int capacity;
};


struct ActivityNode {
    ActivityNode(Activity::ActivityType type, int size = 0) : activityType(type), size(size) {}
    Activity::ActivityType activityType;
    int size;
};


class Workflow {
public:

    Workflow() {

    }

    template<typename A, typename = std::enable_if_t<std::is_same<std::decay_t<A>, Activity>::value>>
    void addActivity(A && activity) {
        if(!activity.getIsCompleted()) {
            pendingActivities.emplace(activity.getId(), std::forward<A>(activity));
        }
    }

    void addWorker(std::unique_ptr<Worker> && worker) {
        if(workers.empty()) {
            workers.push_back(std::move(worker));
        }
        else {
            bool inserted = false;
            for(auto it = workers.begin(); it != workers.end(); it++) {
                if(worker->getCapacity() < (*it)->getCapacity()) {
                    workers.insert(it, std::move(worker));
                    inserted = true;
                    break;
                }
            }

            if(!inserted) {
                workers.emplace_back(std::move(worker));
            }
        }
    }

    bool execute(int activityId) {
        // check if activity is present in pending
        auto it = pendingActivities.find(activityId);
        if(it == pendingActivities.end()) {
            return false;
        }
        auto node = pendingActivities.extract(it);
        Activity a = std::move(node.mapped());
        auto wit = std::lower_bound(workers.begin(), workers.end(), a.getSize(), [](const std::unique_ptr<Worker>& l, int size) {
            return l->getCapacity() < size;
        });

        if(wit == workers.end()) {
            return false;
        }

        assign(std::move(a), (*wit)->getId());

        return true;
    }

    std::list<ActivityNode> getPendingActivityNodesByType() {
        std::unordered_map<Activity::ActivityType, std::list<std::reference_wrapper<const Activity>>> activitiesByType;
        for(const auto& [id, activity] : pendingActivities) {
            activitiesByType[activity.getActivityType()].push_back(std::cref(activity));
        }

        std::list<ActivityNode> activityNodes;
        for(const auto& [type, lstActivities] : activitiesByType) {
            ActivityNode anode(type);
            for(const Activity& a : lstActivities) {
                anode.size += a.getSize();
            }
            activityNodes.push_back(anode);
        }

        return activityNodes;
    }

#if __cplusplus < 202002L
    std::list<Activity> getPendingActivitiesByType(Activity::ActivityType type) {
        std::list<Activity> lstActivities;
        for(const auto& [id, activity] : pendingActivities) {
            if(activity.getActivityType() == type) {
                lstActivities.push_back(activity);
            }
        }

        return lstActivities;
    }
#else
    std::list<Activity> getPendingActivitiesByType(Activity::ActivityType type) {
        std::list<Activity> lstActivities;

        std::ranges::copy_if(
            pendingActivities | std::views::values,
            std::back_inserter(lstActivities),
            [type](const Activity& a) {
                return a.getActivityType() == type;
            }
        );

        return lstActivities;
    }
#endif

    int totalSizeOfPendingActivities() {
        return std::accumulate(pendingActivities.begin(), pendingActivities.end(), 0, [](int init, const auto& p){
            return init + p.second.getSize();
        });
    }


    int getBusiestWorker() {
        if(workers.size() == 0) {
            return -1;
        }

        int busiestId = 0;
        size_t maxTasksAssigned = 0;
        for(const auto& [id, lstActivities] : assignedActivities) {
            if(lstActivities.size() > maxTasksAssigned) {
                maxTasksAssigned = lstActivities.size();
                busiestId = id;
            }
        }
        return busiestId;
    }

private:
    void assign(Activity&& a, Worker::IdType workerId) {
        a.setIsCompleted(true);
        assignedActivities[workerId].push_back(std::move(a));
    }

private:
    std::unordered_map<Activity::IdType, Activity> pendingActivities;
    std::vector<std::unique_ptr<Worker>> workers; // vector of workers ordered by cpacity asc
    std::unordered_map<Worker::IdType, std::list<Activity>> assignedActivities;
};

int main()
{
    Workflow wf;

    wf.addWorker(std::make_unique<Worker>(0, "Worker1", 2));
    wf.addWorker(std::make_unique<Worker>(1, "Worker2", 20));
    wf.addWorker(std::make_unique<Worker>(1, "Worker3", 5));
    wf.addWorker(std::make_unique<Worker>(1, "Worker4", 2));

    Activity a(0, "Activity0", Activity::ActivityType::VALIDATE_INPUT, 1);
    wf.addActivity(a);
    wf.addActivity(Activity(1, "Activity1", Activity::ActivityType::COORDINATE, 3));
    wf.addActivity(Activity(2, "Activity2", Activity::ActivityType::RUN_TASK, 1));
    wf.addActivity(Activity(3, "Activity3", Activity::ActivityType::RUN_TASK, 10));
    wf.addActivity(Activity(4, "Activity4", Activity::ActivityType::RUN_TASK, 4));
    wf.addActivity(Activity(5, "Activity5", Activity::ActivityType::RUN_TASK, 2));

    wf.execute(0);
    wf.execute(1);
    wf.execute(3);

    return 0;
}


