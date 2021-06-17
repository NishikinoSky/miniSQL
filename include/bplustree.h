#pragma onve 
#include <iostream>
#include <vector>
#include <string>
using namespace std;

//����ģ����ʵ��int float string������������ 
//����bplustree�ڵ�������Լ����� 
template <typename T>
class BPTnode{
	public:
		BPTnode(int degree_, bool isleaf_ = false); //���캯�� 
		~BPTnode(); //��������
		bool isroot(); //�ж��Ƿ�Ϊ���ڵ� 
		bool searchkey(T key, unsigned int &index); //�жϽڵ����Ƿ������Ҫ��key
		int addkey(T &key); // ���keyֵ������index
		bool deletekey(unsigned int index); // ɾ��index��Ӧ��key 
		BPTnode* split(T &key); // BPT�ڵ�ķ��� 
		BPTnode* NextLeaf(); //������һ��Ҷ�ڵ� 
	
	public:
		int degree; // bpt�Ķ� 
		bool isleaf; //�ж��Ƿ�ΪҶ�ڵ�
		unsigned int keynum; //�ڵ���key������
		BPTnode* nextleaf; //ָ����һ��Ҷ�ڵ� 
		BPTnode* parent; //ָ�򸸽ڵ�
		vector <T> keys; //���key
		vector <BPTnode*> childs; //���ָ���ӽڵ��ָ��	     
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
	//�ڵ���key����Ϊ0 
	if (keynum == 0){
		index = 0;
		return false;
	}
	//keyС�ڸýڵ�����Сkeyֵ 
	else if (key < keys[0]){
		index = 0;
		return false; 
	}
	//key���ڸýڵ������keyֵ 
	else if (key > keys[keynum - 1]){
		index = keynum;
		return false;
	}
	//���ַ����� 
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
	// ���ڵ����Ѵ��ڸ�keyֵ 
	if (searchkey(key, index)){ //��index�������Ѿ��ı���index��ֵ 
		return -1;
	}
	//����� 
	else {
		if (keynum == 0){
			keys[0] = key;
			keynum ++;
			return index
		}
		//�ı�keyλ���Լ�ָ�� 
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
	// index ������Χ 
	if (index > keynum){
		return false;
	}
	else {
		// Ҷ�ڵ� 
		if (isleaf){
			// �ƶ�key 
			for(int i = index; i < keynum - 1; i++){
				keys[i] = keys[i+1];
			}
			keys.pop_back(); // ɾ���������һ��Ԫ�� 
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
		// �Ұ뿽�����µĽڵ� 
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

//����class bplustree 
template <typename T>
class BPT{
	private:
		typedef BPTnode<T>* Tree;
		
		int degree;
		int level;
		int key_num;
		int node_num;
		int keysize;
		Tree root; // ���ڵ�ָ��
		Tree leafhead; // Ҷ�ڵ���ͷָ��
		struct searchkeyvar{ // �洢Ѱ�ҵ�keyֵ 
			Tree keyptr; // ��Ӧkey��ָ��
			unsigned int index; // key��index
			bool iffind; //�Ƿ��ҵ� 
		};
		
		void initbpt(); // ��bpt��ʼ��
		void search_key(Tree keyptr, T key); // Ѱ����Ҫ��keyֵ�Ľڵ�
		bool insertchangeturn(searchkeyvar &var); // ��������ڵ�ı���
		bool deletechangeturn(searchkeyvar &var); // ɾ����սڵ�ı��� 
	public:
		BPT(); 
		~BPT();
		bool add_key(T &key); // ����һ��keyֵ�󷵻��Ƿ�ɹ�
		bool delete_key(T &key); // ɾ��һ��keyֵ�󷵻��Ƿ�ɹ�
		void drop_tree(Tree node); // ɾ���������������Ƿ�ɹ� 
		
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
	// �ڽڵ����ҵ�keyֵ 
	if (keyptr->search(key, index)){
		//Ҷ�ڵ� 
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
	// δ�ҵ� 
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
	// ��ʼ���� 
	if (root == NULL) initbpt();
	
	searchkeyvar var;
	// �жϸ�key�Ƿ��Ѵ��� 
	search_key(root, key, var);
	if (var.iffind){
		// �Ѵ��� 
		return false;
	}
	else {
		// �������
		var.keyptr->addkey(key);
		// �ڵ����� 
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
			// ���ڵ� 
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
		// �Ǹ��ڵ�
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
	search_key(root, key, var); // ��Ѱ�Ҹ�key�ж��Ƿ����
	// ��key������ 
	if (var.iffind == false){
		return false;
	}
	else {
		// Ϊ���ڵ� 
		if (var.keyptr->isroot()){
			var.keyptr->deletekey(var.index);
			key_num--;
			return deletechangeturn(var);
		}
		else {
			// key��֦�ɽڵ��� 
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
			// key��Ҷ�ڵ��� 
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
	// ���仯 
	if (((var.keyptr->isleaf) && (var.keyptr->keynum >= mid)) || 
		((var.keyptr->isleaf == false) && (degree != 3) && (var.keyptr->keynum >= mid-1)) || 
		((degree == 3) && (var.keyptr->isleaf == false) && (var.keyptr->keynum < 0)) || 
		((var.keyptr->isroot()) && (var.keyptr->keynum > 0))){
			return true;
		}
		
	// ���ڵ� 
	if ((var.keyptr->isroot())) {
		if (var.keyptr->keynum == 0 ){
			if (root->isleaf){
				// ��ɿ��� 
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
	// �Ǹ��ڵ� 
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
		// ֦�ڵ� 
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
