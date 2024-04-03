#pragma once

template<typename T>
void Safe_Erase_Vecter(std::vector<T>& _vec) {
	for (size_t i = 0; i < _vec.size(); ++i) {
		if (_vec[i] != nullptr) {
			delete _vec[i];
		}
	}
	_vec.clear();
}

template<typename T1, typename T2>
void Safe_Erase_Map(std::map<T1, T2>& _map) {
	typename std::map<T1, T2>::iterator iter;
	for (iter = _map.begin(); iter != _map.end(); iter++)
	{
		delete iter->second;
	}
}