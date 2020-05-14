#include "heap.h"

/////////////////heap node
heap_node::heap_node(std::string id0)
{
    this->counter = 0;
    this->id = id0;
    this->left = NULL;
    this->right = NULL;
    this->parent = NULL;
}
heap_node::heap_node(){}
heap_node::~heap_node()
{
    //std::cerr << "destructor heap node: ";
    //this->print_heap_node();
    if (this->left!=NULL) delete this->left;
    if (this->right!=NULL) delete this->right;
}
bool heap_node::isLeftNode()
{
    if (this->parent==NULL)
    {
        //std::cerr << "You're checking the root.\n";
        return true;
    }
    else if (this->parent->left->id==this->id)//an isxuei i isotita -> true giati eimai tou mpampa moy to left paidi
    {
        return true;
    }
    else// if (this->parent->right==this)
    {
        return false;
    }
}
void heap_node::print_heap_node()
{
    std::cout << this->id << " " << this->counter << "\n";
    return;
}
void heap_node::sink() //katadusi
{
    if (this->left==NULL) //den exw paidia na sugkri8w mazi tous, exw den exw left de 8a exw kan right
    {
        return;
    }
    else if (this->right==NULL)//(hn->left!=NULL) exw left alla oxi right paidi
    {
        if (this->left->counter > this->counter) //swap
        {
            int tmp1 = this->left->counter; std::string tmp2 = this->left->id;
            this->left->counter = this->counter; this->left->id = this->id;
            this->counter = tmp1; this->id = tmp2;
            this->left->sink();
        }//else do nothing
    }
    else //exw 2 paidia
    {
        if ((this->left->counter > this->right->counter)&&(this->left->counter > this->counter)) //left > right kai left > me
        {
            int tmp1 = this->left->counter; std::string tmp2 = this->left->id;
            this->left->counter = this->counter; this->left->id = this->id;
            this->counter = tmp1; this->id = tmp2;
            this->left->sink();
        }
        else if ((this->left->counter < this->right->counter)&&(this->right->counter > this->counter)) //right > left kai right > me
        {
            int tmp1 = this->right->counter; std::string tmp2 = this->right->id;
            this->right->counter = this->counter; this->right->id = this->id;
            this->counter = tmp1; this->id = tmp2;
            this->right->sink();
        }
        else if ((this->left->counter==this->right->counter)&&(this->left->counter>this->counter)) //exoun idia timi alla me pernane ara antallasw me to left m
        {
            int tmp1 = this->left->counter; std::string tmp2 = this->left->id;
            this->left->counter = this->counter; this->left->id = this->id;
            this->counter = tmp1; this->id = tmp2;
            this->left->sink();
        }
        
    }
}
void heap_node::swim() //anadusi
{
    if (this->parent==NULL)
    {
        return;
    }
    else //exw gonio kai tha sugkri8w mazi tou
    {
        if (this->counter > this->parent->counter) //o mpampas m exei ligotero metriti apo mena ara swap
        {
            int tmp1 = this->counter; std::string tmp2 = this->id;
            this->counter = this->parent->counter; this->id = this->parent->id;
            this->parent->counter = tmp1; this->parent->id = tmp2;
            this->parent->swim();
        }
        return;
    }
}


///////////////////heap
heap::heap()
{
    this->root = NULL;
    this->size = 0;
    this->last = this->root;
}

heap::~heap() //den prepei na klithei pote giati kanoume delete ton heap prin liksei to programma
{
    //std::cerr << "destroying heap\n";
    delete this->root;
}

void heap::print_heap(heap_node* hn)
{
    if (hn==NULL) return;
    print_heap(hn->left);
    if (hn->parent==NULL)
    {
        std::cerr << "eimai root ";
    }
    else if (hn->isLeftNode()==true)
    {
        std::cerr << "eimai left ";
    }
    else if (hn->isLeftNode()==false)
    {
        std::cerr << "eimai right ";
    }
    hn->print_heap_node();
    print_heap(hn->right);
}

heap_node * heap::prev_last()
{
    if (this->root==NULL) return NULL; //den exw tpt
    else if (root==last) return NULL; //exw 1 stoixeio /*this->last->parent==NULL*/
    else if (this->last->isLeftNode()==false) return this->last->parent->left; //an last einai right paidi, to aristero tou aderfaki einai to proteleutaio
    else if (this->last->isLeftNode()==true) //last itan left
    {
        if (this->size==2) return this->root; //exei 2 stoixeia o swros ara proteleutaia i riza
        else // > 2 stoixeia
        {
            heap_node * curr = this->last;
            //while ((curr->isLeftNode()==true)&&(curr!=root)) //eite anevainw foul aristeros eite eftasa riza
            while ((curr->isLeftNode()==true)&&(curr!=root)) //eite anevainw foul aristeros eite eftasa riza
            {
                //std::cerr << "while "; curr->print_heap_node();
                if (curr->parent!=NULL)
                {
                    curr = curr->parent;
                }
            }
            //std::cerr << "meta ti while "; curr->print_heap_node();
            if (curr==root) //eftasa stin korifi tou dentrou giati last is aristero node, 1o tou katwterou epipedou
            {
                //std::cerr << "eftasa root\n";
                while (curr->right!=NULL)
                {
                    //std::cerr << "while(2) "; curr->print_heap_node();
                    curr = curr->right;
                }
                //std::cerr << "eftasa katw deksia "; curr->print_heap_node();
                return curr;
            }
            else //den itan plires to epipedo
            {
                //std::cerr << "itan plires "; curr->print_heap_node();
                return curr->parent->left->right; //epistrepse ton deksi ksaderfo mou
            }
        }
    }
    else
    {
        std::cerr << "How?\n";
        return NULL;
    }
}

void heap::search(heap_node* hn, std::string s, heap_node** hnn) //O(n) --> can become better using a hash table
{
    //DFS gia na brw to kombo pou exei to info pou thelw, an den uparxei epistrefw NULL
    if (hn==NULL)
    {
        //return NULL;
        return;
    }
    //std::cerr << hn->id << " vs " << s << "\n";
    if (hn->id==s)
    {
        //std::cerr << "found!\n";
        *hnn = hn;
        return;
    }
    search(hn->left, s, hnn);
    search(hn->right, s, hnn);
}

void heap::insert(std::string id0)
{
    heap_node * pou = NULL;
    search(this->root, id0, &pou);
    if (pou == NULL) //den brika pouthena idi auto to string
    {
        if (root==NULL) //1o insert
        {
            //std::cerr << "vazw riza\n";
            root = new heap_node;
            root->id = id0;//edw mpainei to p
            root->counter = 1;
            root->left = NULL;
            root->right = NULL;
            root->parent = NULL;
            this->last = root;
            this->size++;
        }
        else if (last->isLeftNode()==true) //mazi kai i riza
        {
            if (last->parent!=NULL) //den bazw sti riza
            {
                //std::cerr << "to teleutaio pou evala itan left kai oxi i riza\n";
                last->parent->right = new heap_node;
                last->parent->right->id = id0;//edw mpainei to p
                last->parent->right->counter = 1;
                last->parent->right->left = NULL;
                last->parent->right->right = NULL;
                last->parent->right->parent = last->parent;
                this->last = last->parent->right; //to neo last einai to deksi paidi tou mpampa tou proigoumenou last
                this->size++;
            }
            else //paw na valw meta ti riza
            {
                //std::cerr << "vazw meta ti riza\n";
                last->left = new heap_node;
                last->left->id = id0;
                last->left->counter = 1;
                last->left->left = NULL;
                last->left->right = NULL;
                last->left->parent = last;
                this->last = last->left;
                this->size++;
            }
        }
        else if (last->isLeftNode()==false)
        {
            //std::cerr << "to teleutaio pou evala itan right\n";
            heap_node * curr = last;
            //std::cerr << "paw na kanw backtrack\n";
            while(curr->isLeftNode()==false)
            {
                //std::cerr << "elegxw ton mpampa m\n";
                curr = curr->parent;
            }
            if(curr==this->root) //eftasa se riza ara paw sto deksi upodentro
            {
                while (curr->left!=NULL) //pame sto deksi upodentro oso pio **left** ginetai
                {
                    curr = curr->left;
                }//kai xwnomaste
                curr->left = new heap_node;
                curr->left->id = id0;
                curr->left->counter = 1;
                curr->left->left = NULL;
                curr->left->right = NULL;
                curr->left->parent = curr;
                this->last = curr->left;
                this->size++;
            }
            else
            {
                curr = curr->parent; //paw allo 1 curr = curr->parent
                curr->right->left = new heap_node;
                curr->right->left->id = id0;
                curr->right->left->counter = 1;
                curr->right->left->left = NULL;
                curr->right->left->right = NULL;
                curr->right->left->parent = curr->right;
                this->last = curr->right->left;
                this->size++;
            }
        }
        this->last->swim();
    }
    else //uparxei idi auto to string
    {
        pou->counter++;
        pou->swim();
    }
}

/*heap_node **/void heap::pop_root(heap_node * res)
{
    if (this->root==NULL)
    {
        res = NULL;
        return;
    }
    //else exw kati na kanw pop
    else
    {
        //heap_node * res = new heap_node;
        res->id = root->id;
        res->counter = root->counter;
        res->left = NULL;
        res->right = NULL;
        res->parent = NULL;
        
        heap_node * prevlast = this->prev_last(); //krataw poios itan o proteleutaios m
        //std::cerr << "o proteleutaios m einai: "; prevlast->print_heap_node();


        this->root->id = this->last->id; //sti nea riza valw to palio ID tou last
        this->root->counter = this->last->counter; //kai to palio counter tou last  
        //std::cerr << "my new root (pre sink) is: "; this->root->print_heap_node();
        this->root->sink(); //sink to neo root properly
        //std::cerr << "my new root (post sink) is: "; this->root->print_heap_node();      
        //std::cerr << "my new root is: "; this->root->print_heap_node();

        if (this->last==this->root) //if prev_last == null
        {
            this->last = NULL;
            this->root = NULL;
            this->size = 0;
            return;
        }
        else if ((this->last->isLeftNode()==true)&&(this->last->parent!=NULL)) //last is left, not root
        {
            this->last->parent->left = NULL;
        }
        else //it's right
        {
            this->last->parent->right = NULL;
        }
        delete this->last;
        this->last = prevlast;
        //this->print_heap(this->root);
        //sto telos
        size--;
        return;
    }
}