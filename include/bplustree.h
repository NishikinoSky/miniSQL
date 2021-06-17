#pragma once 
#include <iostream>
#include <vector>
#include <string>
using namespace std;

//利用模板类实现int float string三种数据类型 
//定义bplustree节点的属性以及操作 
template <typename T>
class BPTnode{
	public:
		BPTnode(int degree_, bool isleaf_ = false); //构造函数 
		~BPTnode(); //析构函数
		bool isroot(); //判断是否为根节点 
		bool searchkey(T key, unsigned int &index); //判断节点内是否存在需要的key
		int addkey(T &key); // 添加key值并返回index
		bool deletekey(unsigned int index); // 删除index对应的key 
		BPTnode* split(T &key); // BPT节点的分裂 
		BPTnode* NextLeaf(); //返回下一个叶节点 
	
	public:
		int degree; // bpt的度 
		bool isleaf; //判断是否为叶节点
		unsigned int keynum; //节点内key的数量
		BPTnode* nextleaf; //指向下一个叶节点 
		BPTnode* parent; //指向父节点
		vector <T> keys; //存放key
		vector <BPTnode*> childs; //存放指向子节点的指针	     
};

template <class T>
BPTnode<T>::BPTnode(int degree_, bool isleaf_){
	degree = degree_;
	isleaf = isleaf_;
	keynum = 0;
	nextleaf = NULL;
	parent = NULL;
	
	for (int i = 0; i < degree + 1; i++){
		keys[i] = T();
		childs[i] = NULL;
	}
}

template <class T>
BPTnode<T>::~BPTnode(){
	
}

template <class T>
bool BPTnode<T>::isroot(){
	if (parent != NULL) return false;
	else return true;
} 

template <class T>
bool BPTnode<T>::searchkey(T key, unsigned int &index){
	//节点内key数量为0 
	if (keynum == 0){
		index = 0;
		return false;
	}
	//key小于该节点内最小key值 
	else if (key < keys[0]){
		index = 0;
		return false; 
	}
	//key大于该节点内最大key值 
	else if (key > keys[keynum - 1]){
		index = keynum;
		return false;
	}
	//二分法搜索 
	else {
		int left = 0;
		int right = keynum -1;
		int i = (left + right) / 2;
		while (right >= left){
			if (key == keys[i]){
				index = i;
				return true;
			}
			else if (key < keys[i]){
				right = i;
			}
			else if (key > keys[i]){
				left = i;
			}
		}
	}
			
	return false;
}

template <class T>
int BPTnode<T>::addkey(T &key){
	unsigned int index = 0;
	// 若节点内已存在该key值 
	if (searchkey(key, index)){ //对index的引用已经改变了index的值 
		return -1;
	}
	//可添加 
	else {
		if (keynum == 0){
			keys[0] = key;
			keynum ++;
			return index
		}
		//改变key位置以及指针 
		else{
			for (int i = keynum; i > index; i--){
				keys[i] = keys[i-1];
			}
			keys[index] = key;
		
			for (int i = keynum + 1; i > index + 1; i--){
				childs[i] = childs[i-1];
			}
			childs[index + 1] = NULL;
			keynum++;
			return index;
		}
	}
	
	return 0;
}

template <class T>
bool BPTnode<T>::deletekey(unsigned int index){
	// index 超过范围 
	if (index > keynum){
		return false;
	}
	else {
		// 叶节点 
		if (isleaf){
			// 移动key 
			for(int i = index; i < keynum - 1; i++){
				keys[i] = keys[i+1];
			}
			keys.pop_back(); // 删除容器最后一个元素 
		}
		else {
			for(int i = index; i < keynum - 1; i++){
				keys[i] = keys[i+1];
			}
			keys.pop_back();
			
			for(int i = index + 1; i < keynum; i++){
				childs[i] = childs[i+1];
			}
			childs[num] = NULL;
		}
		
		keynum = keynum - 1;
		return true;
	}
	
	return false;
} 

template <class T>
BPTnode<T>* BPTnode<T>::split(T &key){
	BPTnode* newnode = new BPTnode(degree, this->isleaf);
	int mid = (degree - 1) / 2;
	if (isleaf){
		key = keys[mid + 1];
		// 右半拷贝至新的节点 
		for (int i = mid + 1; i < degree; i++){
			newnode->keys[i-mid-i] = keys[i];
			keys.pop_back();
		}
		
		newnode->nextleaf = this->nextleaf;
		this->nextleaf = newnode;
		newnode->parent = this->parent;
		newnode->keynum = mid;
		this->keynum = mid + 1;
	}
	else {
		key = keys[mid];
		for (int i = mid + 1; i < degree + 1; i++){
			newnode->childs[i-mid-1] = this->childs[i];
			newnode->childs[i-mid-1]->parent = newnode;
			this->childs[i] = NULL;
		}
		for (int i = mid + 1; i < degree; i++){
			newnode->keys[i-mid-1] = this->keys[i];
			this->keys.pop_back();
		}
		
		this->keys.pop_back();
		newnode->parent = this->parent;
		newnode->keynum = mid;
		this->keynum = mid;
	}
	
	return newnode; 
}

template <class T>
BPTnode<T>* BPTnode<T>::NextLeaf(){
	return nextleaf;
}

//定义class bplustree 
template <typename T>
class BPT{
	private:
		typedef BPTnode<T>* Tree;
		
		int degree;
		int level;
		int key_num;
		int node_num;
		int keysize;
		Tree root; // 根节点指针
		Tree leafhead; // 叶节点左部头指针
		struct searchkeyvar{ // 存储寻找的key值 
			Tree keyptr; // 对应key的指针
			unsigned int index; // key的index
			bool iffind; //是否找到 
		};
		
		void initbpt(); // 对bpt初始化
		void search_key(Tree keyptr, T key); // 寻找需要的key值的节点
		bool insertchangeturn(searchkeyvar &var); // 插入后满节点改变树
		bool deletechangeturn(searchkeyvar &var); // 删除后空节点改变树 
	public:
		BPT(); 
		~BPT();
		bool add_key(T &key); // 插入一个key值后返回是否成功
		bool delete_key(T &key); // 删除一个key值后返回是否成功
		void drop_tree(Tree node); // 删除整棵树并返回是否成功 
		
};

template <class T>
void BPT<T>::initbpt(){
	root = new BPTnode<T>(degree, true);
	level = 1;
	key_num = 0;
	node_num = 1;
	leafhead = root;
}

template <class T>
void BPT<T>::search_key(Tree keyptr, T key, searchkeyvar &var){
	unsigned int index = 0;
	// 在节点内找到key值 
	if (keyptr->search(key, index)){
		//叶节点 
		if (keyptr->isleaf){
			var.index = index;
			var.keyptr = keyptr;
			var.iffind = true;
		}
		else {
			keyptr = keyptr->childs[index+1];
			while(keyptr->isleaf == false){
				keyptr = keyptr->childs[0];
			}
			
			var.keyptr = keyptr;
			var.index = 0;
			var.iffind = true;
		}
	}
	// 未找到 
	else {
		if (keyptr->isleaf){
			var.keyptr = keyptr;
			var.index = index;
			var.iffind =false;
		}
		else {
			keyptr = keyptr->childs[index];
			search_key(keyptr, key, var);
		}
	}
}

template <class T>
bool BPT<T>::add_key(T &key){
	// 初始化树 
	if (root == NULL) initbpt();
	
	searchkeyvar var;
	// 判断该key是否已存在 
	search_key(root, key, var);
	if (var.iffind){
		// 已存在 
		return false;
	}
	else {
		// 插入操作
		var.keyptr->addkey(key);
		// 节点已满 
		if (var.keyptr->keynum == degree){
			insertchangeturn(var);
		}
		key_num++;
		return true;
	}
	
	return false;
}

template <class T>
bool BPT<T>::insertchangeturn(searchkeyvar &var){
		Tree newnode = var.keyptr->split(key);
		node_num++;
			// 根节点 
		if (var.keyptr->isroot()){
			Tree root = new BPTnode<T>(degree, false);
			level++;
			node_num++;
			this->root = root;
			var.keyptr->parent = root;
			root->addkey(key);
			root->childs[0] = var.keyptr;
			root->childs[1] = newnode;
			return true;
		}
		// 非根节点
		else {
			Tree parent = var.keyptr->parent;
			unsigned int index = parent->addkey(key);
			parent->childs[index+1] = newnode;
			newnode->parent = parent;
			if (parent->keynum = degree){
				return insertchangeturn(parent);
			}
			return true;
		} 
			
	return false;
} 

template <class T>
bool BPT<T>::delete_key(T &key){
	searchkeyvar var;
	search_key(root, key, var); // 先寻找该key判断是否存在
	// 该key不存在 
	if (var.iffind == false){
		return false;
	}
	else {
		// 为根节点 
		if (var.keyptr->isroot()){
			var.keyptr->deletekey(var.index);
			key_num--;
			return deletechangeturn(var);
		}
		else {
			// key在枝干节点上 
			if (var.index == 0 && leafhead != var.keyptr){
				unsigned int index = 0;
				Tree updateparent = var.keyptr->parent;
				bool ifbranch = updateparent->searchkey(key, index);
				while (ifbranch == false){
					updateparent = updateparent->parent;
					ifbranch = updateparent->searchkey(key, index);
				}
				key_num--;
				updateparent->keys[index] = var.keyptr->keys[1];
				var.keyptr->deletekey(var.index);
				return deletechangeturn(var);				
			}
			// key在叶节点上 
			else {
				var.keyptr->deletekey(var.index);
				key_num--;
				return deletechangeturn(var); 
			}
		}
	}
	
	return false; 
}

template <class T>
bool BPT<T>::deletechangeturn(searchkeyvar &var){
	int mid = (degree - 1) / 2;
	// 不变化 
	if (((var.keyptr->isleaf) && (var.keyptr->keynum >= mid)) || 
		((var.keyptr->isleaf == false) && (degree != 3) && (var.keyptr->keynum >= mid-1)) || 
		((degree == 3) && (var.keyptr->isleaf == false) && (var.keyptr->keynum < 0)) || 
		((var.keyptr->isroot()) && (var.keyptr->keynum > 0))){
			return true;
		}
		
	// 根节点 
	if ((var.keyptr->isroot())) {
		if (var.keyptr->keynum == 0 ){
			if (root->isleaf){
				// 变成空树 
				delete var.keyptr;
				root = NULL;
				leafhead = NULL;
				level--;
				node_num--;
			}
			else {
				root = var.keyptr->childs[0];
				root->parent = NULL;
				delete var.keyptr;
				level--;
				node_num--; 
			}
		}
	}
	// 非根节点 
	else {
		Tree parent = var.keyptr->parent;
		Tree brother = NULL;
		if (var.keyptr->isleaf){
			unsigned int index = 0;
			parent->searchkey(var.keyptr->keys[0], index);
			
			if ((parent->childs[0] != var.keyptr) && (index+1 == parent->keynum)){
				brother = parent->childs[index];
				if (brother->num > mid){
					for (int i = var.keyptr->keynum; i > 0; i--){
						var.keyptr->keys[i] = var.keyptr->keys[i-1];
					}
					var.keyptr->keys[0] = brother->keys[brother->keynum - 1];
					brother->deletekey(brother->keynum-1);
					var.keyptr->keynum++;
					parent->keys[index] = var.keyptr->keys[0];
					return true;
				} 
				
				else {
					parent->deletekey(index);
					for (int i = 0; i < var.keyptr->keynum; i++){
						brother->keys[i+brother->keynum] = var.keyptr->keys[i];
					}
					brother->num = brother->num + var.keyptr->num;
					brother->nextleaf = var.keyptr->nextleaf;
					
					delete var.keyptr;
					node_num--;
					return deletechangeturn(parent);
				}
			}
			
			else {
				if (parent->childs[0] == var.keyptr){
					brother = parent->childs[1];
				} 
				else {
					brother = parent->childs[index+2];
				}
				
				if (brother->num > mid){
					var.keyptr->keys[var.keyptr->keynum] = brother->keys[0];
					var.keyptr->keynum++;
					brother->deletekey(0);
					if (parent->childs[0] == var.keyptr){
					parent->keys[0] = brother->keys[0];
					}
					else {
						parent->keys[index+1] = brother->keys[0];
					}
					return true;
				}
				else {
					for (int i = 0; i < brother->keynum; i++){
						var.keyptr->keys[var.keyptr->keynum+i] = brother->keys[i];
					}
					if (var.keyptr == parent->childs[0]) parent->deletekey(0);
					else parent->deletekey(index+1);
					var.keyptr->keynum = var.keyptr->keynum + brother->keynum;
					var.keyptr->nextleaf = brother->nextleaf;
					delete brother;
					node_num--;
					
					return deletechangeturn(parent);
				}
			}
		}
		// 枝节点 
		else {
			unsigned int index = 0;
			parent->searchkey(var.keyptr->childs[0]->keys[0], index);
			if ((parent->childs[0] != var.keyptr) && (index+1 == parent->keynum)){
				brother = parent->childs[index];
				if (brother->keynum > mid - 1){
					var.keyptr->childs[var.keyptr->keynum+1] = var.keyptr->childs[var.keyptr->keynum];
					for (int i = var.keyptr->keynum; i > 0; i--){
						var.keyptr->childs[i] = var.keyptr->childs[i-1];
					}
					var.keyptr->childs[0] = brother->childs[brother->keynum];
					var.keyptr->keys[0] = parent->keys[index];
					var.keyptr->keynum++;
					parent->keys[index] = brother->keys[brother->keynum-1];
					
					if (brother->childs[brother->keynum]){
						brother->childs[brother->keynum]->parent = var.keyptr; 
					}
					brother->deletekey(brother->keynum-1);
					return true; 
				}
				
				else{
					brother->keys[brother->keynum] = parent->keys[index];
					parent->deletekey(index);
					brother->keynum++;
					
					for(int i = 0; i < var.keyptr->keynum; i++){
						brother->childs[brother->keynum+i] = var.keyptr->childs[i];
						brother->keys[brother->keynum+i] = var.keyptr->keys[i];
						brother->childs[brother->keynum+i]->parent = brother;
					}
					brother->childs[brother->keynum+var.keyptr->keynum] = var.keyptr->childs[var.keyptr->keynum];
					brother->childs[brother->keynum+var.keyptr->keynum]->parent = brother;
					brother->keynum = brother->keynum + var.keyptr->keynum;
					
					delete var.keyptr;
					node_num--;
					
					return  deletechangeturn(parent);
				} 
			}
			
			else {
				if (parent->childs[0] == var.keyptr) brother = parent->childs[1];
				else brother = parent->childs[index+2];
				
				if (brother->keynum > mid-1){
					var.keyptr->childs[var.keyptr->keynum+1] = brother->childs[0];
					var.keyptr->childs[var.keyptr->keynum+1]->parent = var.keyptr;
					var.keyptr->keys[var.keyptr->keynum] = brother->keys[0];
					var.keyptr->keynum++;
					
					if (var.keyptr == parent->childs[0]) parent->keys[0] = brother->keys[0];
					else parent->keys[index+1] = brother->keys[0];
					
					brother->childs[0] = brother->childs[1];
					brother->deletekey(0);
					
					return true;
				}
				
				else {
					var.keyptr->keys[var.keyptr->keynum] = parent->keys[index];
					if(var.keyptr == parent->childs[0]) parent->deletekey(0);
					else parent->deletekey(index+1);
					
					var.keyptr->keynum++;
					
					for(int i = 0; i < brother->keynum; i++){
						var.keyptr->childs[var.keyptr->keynum+i] = brother->childs[i];
						var.keyptr->keys[var.keyptr->keynum+i] = brother->keys[i];
						var.keyptr->childs[var.keyptr->keynum+i]->parent = var.keyptr;
					} 
					
					var.keyptr->childs[var.keyptr->keynum+brother->keynum] = brother->cgilds[brother->keynum];
					var.keyptr->childs[var.keyptr->keynum+brother->keynum]->parent = var.keyptr;
					
					var.keyptr->keynum = var.keyptr->keynum + brother->keynum;
					
					delete brother;
					node_num++;
					
					return deletechangeturn(parent); 
				}
			}
		}
	}
	
	return false;
}

template <class T>
void BPT<T>::drop_tree(Tree node){
	if (node == NULL) return;
	
	if (node->isleaf == false){
		for (int i = 0; i <= node->keynum; i++){
			drop_tree(node->childs[i]);
			node->childs[i] = NULL;
		}
	}
	
	delete node;
	node_num--;
	return;
}
