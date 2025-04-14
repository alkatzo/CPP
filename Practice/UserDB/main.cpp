#include <unordered_map>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <chrono>
#include <functional>


class User {
public:
    User(int id, const std::string &name, const std::chrono::year_month_day &dob, const std::chrono::year_month_day &enrollment_date, const std::chrono::sys_time<std::chrono::milliseconds> &last_access) : id(id),
        name(name),
        dob(dob),
        enrollment_date(enrollment_date),
        last_access(last_access)
    {}

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

    std::chrono::year_month_day getDob() const
    {
        return dob;
    }

    void setDob(const std::chrono::year_month_day &newDob)
    {
        dob = newDob;
    }

    std::chrono::year_month_day getEnrollmentDate() const
    {
        return enrollment_date;
    }

    void setEnrollmentDate(const std::chrono::year_month_day &newEnrollment_date)
    {
        enrollment_date = newEnrollment_date;
    }

    std::chrono::sys_time<std::chrono::milliseconds> getLastAccess() const
    {
        return last_access;
    }

    void setLastAccess(const std::chrono::sys_time<std::chrono::milliseconds> &newLast_access)
    {
        last_access = newLast_access;
    }


private:
    int id;
    std::string name;
    std::chrono::year_month_day dob;
    std::chrono::year_month_day enrollment_date;
    std::chrono::sys_time<std::chrono::milliseconds> last_access;
};


bool operator==(const User& l, const User& r) {
    return
        l.getId() == r.getId() &&
        l.getDob() == r.getDob() &&
        l.getEnrollmentDate() == r.getEnrollmentDate() &&
        l.getLastAccess() == r.getLastAccess() &&
        l.getName() == r.getName();
}

std::list<User>* compareUsers(std::list<User>& lstDB, std::list<User>& lstUpd) {

    std::list<User>* res = new std::list<User>[2];

    std::list<User>& newUsers = res[0];
    std::list<User>& updUsers = res[1];

    if(lstDB.empty() || lstUpd.empty()) {
        return {};
    }

    using UserIDType = decltype(std::declval<User>().getId());
    std::unordered_map<UserIDType, std::reference_wrapper<const User>> db;
    std::unordered_map<UserIDType, std::reference_wrapper<const User>> updUserMap;

    // build the db
    for(const auto& user : lstDB) {
        db.emplace(user.getId(), std::cref(user));
    }

    for(const auto& u : lstUpd) {
        if(u.getId() == 0) {
            newUsers.push_back(u);
        }
        else {
            const auto it = db.find(u.getId());
            if(it != db.end() && u != (*it).second) {
                updUserMap.emplace(u.getId(), std::cref(u));
            }
        }
    }

    for(const auto& updUserMapEntry : updUserMap) {
        updUsers.push_back(updUserMapEntry.second);
    }

    return res;
}

int main()
{
    using namespace std::chrono;

    sys_time<milliseconds> now_ms = time_point_cast<milliseconds>(system_clock::now());

    std::list<User> db({
                        User(1, "A", year{2000}/1/1, year{2020}/1/1, now_ms - 20s),
                        User(2, "B", year{2000}/1/2, year{2020}/1/1, now_ms - 20s),
                        User(3, "C", year{2000}/1/3, year{2020}/1/1, now_ms - 20s),
                        User(4, "D", year{2000}/1/4, year{2020}/1/1, now_ms - 21s),
                        User(5, "E", year{2000}/1/5, year{2020}/1/1, now_ms - 20s),
                        User(6, "F", year{2000}/1/6, year{2020}/1/1, now_ms - 20s),
                        User(7, "G", year{2000}/1/7, year{2020}/1/1, now_ms - 20s),
                        });


    std::list<User> upd({
                        User(1, "AA", year{2000}/1/1, year{2020}/1/1, now_ms - 20s),
                        User(2, "B", year{2000}/1/2, year{2020}/1/1, now_ms - 20s),
                        User(7, "GG", year{2000}/1/7, year{2020}/1/1, now_ms - 20s),
                        User(0, "AAA", year{2000}/1/1, year{2020}/1/1, now_ms - 20s),
                        User(7, "G", year{2000}/1/7, year{2020}/1/1, now_ms - 20s),
                        User(7, "G", year{2000}/1/7, year{2020}/1/1, now_ms - 20s)
                        });

    auto res = compareUsers(db, upd);

    std::list<User> newUsers = res[0];
    std::list<User> updUsers = res[1];

    return 0;
}




