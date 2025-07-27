#include "feed.h"

namespace feed {

Feed::Feed() {}

void Feed::start() {
    if(!thread_opt.has_value()) {
        thread_opt.emplace([](){

        });
    }
}

void Feed::stop() {

}

}
