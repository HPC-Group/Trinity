#ifndef CMDQUEUE_H
#define CMDQUEUE_H

#include <string>
#include <queue>
#include <memory>

#include <core/SilverBulletBase.h>
#include <core/Math/Vectors.h>

namespace Core {

    class AbstrCmd{
        public:
            AbstrCmd(std::string cmd): m_sCmd(cmd){};
            virtual ~AbstrCmd(){ }

            std::string getCmd() const { return m_sCmd; }
        protected:
            std::string m_sCmd;
    };

    template <typename T>
    class TemplateCmd : public AbstrCmd{
        public:
            TemplateCmd(std::string cmd, T data):AbstrCmd(cmd),m_tData(data){
            };
            virtual ~TemplateCmd(){ }

            T getData() {return m_tData; }
        protected:
            std::string m_sCmd;
            T           m_tData;
    };

    class Vec3fCmd : public TemplateCmd<Core::Math::Vec3f>{         public: Vec3fCmd(std::string cmd, Core::Math::Vec3f data):TemplateCmd(cmd,data){}; virtual ~Vec3fCmd(){};};
    class Vec2uiCmd : public TemplateCmd<Core::Math::Vec2ui>{       public: Vec2uiCmd(std::string cmd, Core::Math::Vec2ui data):TemplateCmd(cmd,data){}; virtual ~Vec2uiCmd(){};};
    class FloatCmd : public TemplateCmd<float>{                     public: FloatCmd(std::string cmd, float data):TemplateCmd(cmd,data){}; virtual ~FloatCmd(){};};
    class Uint16Cmd : public TemplateCmd<uint16_t>{                 public: Uint16Cmd(std::string cmd, uint16_t data):TemplateCmd(cmd,data){}; virtual ~Uint16Cmd(){};};
    class ShortCmd : public TemplateCmd<short>{                     public: ShortCmd(std::string cmd, short data):TemplateCmd(cmd,data){}; virtual ~ShortCmd(){};};

    class CmdQueue{
        public:
            CmdQueue(){};
            ~CmdQueue(){};

            void push(std::shared_ptr<AbstrCmd> cmd) {m_cmdQueue.push(cmd);}
            std::shared_ptr<AbstrCmd> pop() { std::shared_ptr<AbstrCmd> c = m_cmdQueue.front(); m_cmdQueue.pop(); return c;}
            int size() const {return m_cmdQueue.size();}
        private:
            std::queue<std::shared_ptr<AbstrCmd>> m_cmdQueue;
    };
}

#endif // CMDQUEUE_H
