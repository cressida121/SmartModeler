#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

template <typename T>
class EventDispatcher {
public:
	virtual ~EventDispatcher() = default;

	void AddEventListener(std::shared_ptr<T> &listener) {
		m_listeners.push_back(listener);
	}

	void RemoveEventListener(std::shared_ptr<T> &listener) {
		for(auto it = m_listeners.begin(); it != m_listeners.end(); ) {
			if (!it->expired() && it->lock() == listener) {
				it = m_listeners.erase(it);
				return;
			} else {
				++it;
			}
		}
	}

	void DispatchEvent(std::function<void(std::shared_ptr<T>)> eventCallback) {
		for (auto &listener : m_listeners) {
			if (!listener.expired() && eventCallback) {
				eventCallback(listener.lock());
			}
		}
	}

protected:
	std::vector<std::weak_ptr<T>> m_listeners;
};