#include "tarapch.h"
#include "Uniform.h"



namespace Tara {

    uint32_t Uniform::S_Allocations = 0;;

    UniformData::UniformData(float f)
    {
        //manually assign to Float1
        this->FloatVector.Data = nullptr;
        this->Float1 = f;
    }

    UniformData::~UniformData()
    {
    }

    UniformData::FloatVectorType::FloatVectorType()
        :Data(nullptr), Count(0), Elements(1)
    {}
    UniformData::FloatVectorType::FloatVectorType(const float* data, int32_t count, int32_t elements)
        : Count(count), Elements(elements)
    {
        Data = new float[count * elements];
        Uniform::S_Allocations++;
        memcpy_s(Data, Count * Elements * sizeof(float), data, count * elements * sizeof(float));
    }
    UniformData::FloatVectorType::FloatVectorType(const FloatVectorType& other)
        : Count(other.Count), Elements(other.Elements)
    {
        Data = new float[Count * Elements];
        Uniform::S_Allocations++;
        memcpy_s(Data, Count * Elements * sizeof(float), other.Data, Count * Elements * sizeof(float));
    }
    UniformData::FloatVectorType::FloatVectorType(FloatVectorType&& other)
        : Count(other.Count), Elements(other.Elements)
    {
        Data = other.Data;
        other.Data = nullptr;
        other.Count = 0;
        other.Elements = 1;
    }
    UniformData::FloatVectorType& UniformData::FloatVectorType::operator=(const FloatVectorType& other)
    {
        //Copy Data
        if (Data) {
            delete[] Data;
            Uniform::S_Allocations--;
        }
        Count = other.Count;
        Elements = other.Elements;
        Data = new float[Count * Elements];
        Uniform::S_Allocations++;
        memcpy_s(Data, Count * Elements * sizeof(float), other.Data, Count * Elements * sizeof(float));
        return *this;
    }
    UniformData::FloatVectorType& UniformData::FloatVectorType::operator=(FloatVectorType&& other)
    {
        //Move Data
        if (Data) {
            delete[] Data;
            Uniform::S_Allocations--;
        }
        Count = other.Count;
        Elements = other.Elements;
        Data = other.Data;
        other.Data = nullptr;
        other.Count = 0;
        other.Elements = 1;
        return *this;
    }
    UniformData::FloatVectorType::~FloatVectorType()
    {
        if (Data) {
            delete[] Data;
            Uniform::S_Allocations--;
        }
        Data = nullptr;
    }


    UniformData::IntVectorType::IntVectorType()
        :Data(nullptr), Count(0), Elements(1)
    {}
    UniformData::IntVectorType::IntVectorType(const int32_t* data, int32_t count, int32_t elements)
        : Count(count), Elements(elements)
    {
        Data = new int32_t[count * elements];
        Uniform::S_Allocations++;
        memcpy_s(Data, Count * Elements * sizeof(int32_t), data, count * elements * sizeof(int32_t));
    }
    UniformData::IntVectorType::IntVectorType(const IntVectorType& other)
        : Count(other.Count), Elements(other.Elements)
    {
        Data = new int32_t[Count * Elements];
        Uniform::S_Allocations++;
        memcpy_s(Data, Count * Elements * sizeof(int32_t), other.Data, Count * Elements * sizeof(int32_t));
    }
    UniformData::IntVectorType::IntVectorType(IntVectorType&& other)
        : Count(other.Count), Elements(other.Elements)
    {
        Data = other.Data;
        other.Data = nullptr;
        other.Count = 0;
        other.Elements = 1;
    }
    UniformData::IntVectorType& UniformData::IntVectorType::operator=(const IntVectorType& other)
    {
        //Copy Data
        if (Data) {
            delete[] Data;
            Uniform::S_Allocations--;
        }
        Count = other.Count;
        Elements = other.Elements;
        Data = new int32_t[Count * Elements];
        Uniform::S_Allocations++;
        memcpy_s(Data, Count * Elements * sizeof(int32_t), other.Data, Count * Elements * sizeof(int32_t));
        return *this;
    }
    UniformData::IntVectorType& UniformData::IntVectorType::operator=(IntVectorType&& other)
    {
        //Move Data
        if (Data) {
            delete[] Data;
            Uniform::S_Allocations--;
        }
        Count = other.Count;
        Elements = other.Elements;
        Data = other.Data;
        other.Data = nullptr;
        other.Count = 0;
        other.Elements = 1;
        return *this;
    }
    UniformData::IntVectorType::~IntVectorType()
    {
        if (Data) {
            delete[] Data;
            Uniform::S_Allocations--;
        }
        Data = nullptr;
    }


    UniformData::UIntVectorType::UIntVectorType()
        :Data(nullptr), Count(0), Elements(1)
    {}
    UniformData::UIntVectorType::UIntVectorType(const uint32_t* data, int32_t count, int32_t elements)
        : Count(count), Elements(elements)
    {
        Data = new uint32_t[count * elements];
        Uniform::S_Allocations++;
        memcpy_s(Data, Count * Elements * sizeof(uint32_t), data, count * elements * sizeof(uint32_t));
    }
    UniformData::UIntVectorType::UIntVectorType(const UIntVectorType& other)
        : Count(other.Count), Elements(other.Elements)
    {
        Data = new uint32_t[Count * Elements];
        Uniform::S_Allocations++;
        memcpy_s(Data, Count * Elements * sizeof(uint32_t), other.Data, Count * Elements * sizeof(uint32_t));
    }
    UniformData::UIntVectorType::UIntVectorType(UIntVectorType&& other)
        : Count(other.Count), Elements(other.Elements)
    {
        Data = other.Data;
        other.Data = nullptr;
        other.Count = 0;
        other.Elements = 1;
    }
    UniformData::UIntVectorType& UniformData::UIntVectorType::operator=(const UIntVectorType& other)
    {
        //Copy Data
        if (Data) {
            delete[] Data;
            Uniform::S_Allocations--;
        }
        Count = other.Count;
        Elements = other.Elements;
        Data = new uint32_t[Count * Elements];
        Uniform::S_Allocations++;
        memcpy_s(Data, Count * Elements * sizeof(uint32_t), other.Data, Count * Elements * sizeof(uint32_t));
        return *this;
    }
    UniformData::UIntVectorType& UniformData::UIntVectorType::operator=(UIntVectorType&& other)
    {
        //Move Data
        if (Data) {
            delete[] Data;
            Uniform::S_Allocations--;
        }
        Count = other.Count;
        Elements = other.Elements;
        Data = other.Data;
        other.Data = nullptr;
        other.Count = 0;
        other.Elements = 1;
        return *this;
    }
    UniformData::UIntVectorType::~UIntVectorType()
    {
        if (Data) {
            delete[] Data;
            Uniform::S_Allocations--;
        }
        Data = nullptr;
    }

    Uniform::Uniform()
        :DataType(UniformType::Float1), Data(1)
    {
    }

    Uniform::Uniform(UniformType type, const UniformData& data)
        :DataType(type), Data(0)
    {
        SetData(data);
    }

    Uniform::Uniform(UniformType type, UniformData&& data)
        :DataType(type), Data(0)
    {
        SetData(std::move(data));
    }

    Uniform::Uniform(const Uniform& other)
        :DataType(other.DataType), Data(0)
    {
        SetData(other.Data);
    }

    Uniform::Uniform(Uniform&& other)
        :DataType(other.DataType), Data(0)
    {
        SetData(std::move(other.Data));
    }

    Uniform& Uniform::operator=(const Uniform& other)
    {
        ClearOldData();
        DataType = other.DataType;
        SetData(other.Data);
        return *this;
    }

    Uniform& Uniform::operator=(Uniform&& other)
    {
        ClearOldData();
        DataType = other.DataType;
        SetData(std::move(other.Data));
        return *this;
    }

    Uniform::~Uniform()
    {
        ClearOldData();
    }

    void Uniform::SetData(const UniformData& data)
    {
        switch (DataType) {
        case UniformType::Float1: Data.Float1 = data.Float1; break;
        case UniformType::Float2: Data.Float2 = data.Float2; break;
        case UniformType::Float3: Data.Float3 = data.Float3; break;
        case UniformType::Float4: Data.Float4 = data.Float4; break;
        case UniformType::Int1: Data.Int1 = data.Int1; break;
        case UniformType::Int2: Data.Int2 = data.Int2; break;
        case UniformType::Int3: Data.Int3 = data.Int3; break;
        case UniformType::Int4: Data.Int4 = data.Int4; break;
        case UniformType::UInt1: Data.UInt1 = data.UInt1; break;
        case UniformType::UInt2: Data.UInt2 = data.UInt2; break;
        case UniformType::UInt3: Data.UInt3 = data.UInt3; break;
        case UniformType::UInt4: Data.UInt4 = data.UInt4; break;
            //Intentional Fallthrough
        case UniformType::Int1Vector:
        case UniformType::Int2Vector:
        case UniformType::Int3Vector:
        case UniformType::Int4Vector:
        {
            Data.IntVector = data.IntVector;
            break;
        }
        //Intentional Fallthrough
        case UniformType::UInt1Vector:
        case UniformType::UInt2Vector:
        case UniformType::UInt3Vector:
        case UniformType::UInt4Vector:
        {
            Data.UIntVector = data.UIntVector;
            break;
        }
        case UniformType::Float1Vector:
        case UniformType::Float2Vector:
        case UniformType::Float3Vector:
        case UniformType::Float4Vector:
        case UniformType::Matrix2Vector:
        case UniformType::Matrix3Vector:
        case UniformType::Matrix4Vector:
        case UniformType::Matrix2x3Vector:
        case UniformType::Matrix3x2Vector:
        case UniformType::Matrix2x4Vector:
        case UniformType::Matrix4x2Vector:
        case UniformType::Matrix3x4Vector:
        case UniformType::Matrix4x3Vector:
        {
            Data.FloatVector = data.FloatVector;
            break;
        }
        default: Data.Float1 = data.Float1;
        }
    }

    void Uniform::SetData(UniformData&& data)
    {
        switch (DataType) {
        case UniformType::Float1: Data.Float1 = data.Float1; break;
        case UniformType::Float2: Data.Float2 = data.Float2; break;
        case UniformType::Float3: Data.Float3 = data.Float3; break;
        case UniformType::Float4: Data.Float4 = data.Float4; break;
        case UniformType::Int1: Data.Int1 = data.Int1; break;
        case UniformType::Int2: Data.Int2 = data.Int2; break;
        case UniformType::Int3: Data.Int3 = data.Int3; break;
        case UniformType::Int4: Data.Int4 = data.Int4; break;
        case UniformType::UInt1: Data.UInt1 = data.UInt1; break;
        case UniformType::UInt2: Data.UInt2 = data.UInt2; break;
        case UniformType::UInt3: Data.UInt3 = data.UInt3; break;
        case UniformType::UInt4: Data.UInt4 = data.UInt4; break;
            //Intentional Fallthrough
        case UniformType::Int1Vector:
        case UniformType::Int2Vector:
        case UniformType::Int3Vector:
        case UniformType::Int4Vector:
        {
            Data.IntVector = std::move(data.IntVector);
            break;
        }
        //Intentional Fallthrough
        case UniformType::UInt1Vector:
        case UniformType::UInt2Vector:
        case UniformType::UInt3Vector:
        case UniformType::UInt4Vector:
        {
            Data.UIntVector = std::move(data.UIntVector);
            break;
        }
        case UniformType::Float1Vector:
        case UniformType::Float2Vector:
        case UniformType::Float3Vector:
        case UniformType::Float4Vector:
        case UniformType::Matrix2Vector:
        case UniformType::Matrix3Vector:
        case UniformType::Matrix4Vector:
        case UniformType::Matrix2x3Vector:
        case UniformType::Matrix3x2Vector:
        case UniformType::Matrix2x4Vector:
        case UniformType::Matrix4x2Vector:
        case UniformType::Matrix3x4Vector:
        case UniformType::Matrix4x3Vector:
        {
            Data.FloatVector = std::move(data.FloatVector);
            break;
        }
        default: Data.Float1 = data.Float1;
        }
    }

    void Uniform::ClearOldData()
    {
        switch (DataType) {
        //Intentional Fallthrough
        case UniformType::Int1Vector:
        case UniformType::Int2Vector:
        case UniformType::Int3Vector:
        case UniformType::Int4Vector:
        {
            Data.IntVector.~IntVectorType();
            break;
        }
        //Intentional Fallthrough
        case UniformType::UInt1Vector:
        case UniformType::UInt2Vector:
        case UniformType::UInt3Vector:
        case UniformType::UInt4Vector:
        {
            Data.UIntVector.~UIntVectorType();
            break;
        }
        case UniformType::Float1Vector:
        case UniformType::Float2Vector:
        case UniformType::Float3Vector:
        case UniformType::Float4Vector:
        case UniformType::Matrix2Vector:
        case UniformType::Matrix3Vector:
        case UniformType::Matrix4Vector:
        case UniformType::Matrix2x3Vector:
        case UniformType::Matrix3x2Vector:
        case UniformType::Matrix2x4Vector:
        case UniformType::Matrix4x2Vector:
        case UniformType::Matrix3x4Vector:
        case UniformType::Matrix4x3Vector:
        {
            Data.FloatVector.~FloatVectorType();
            break;
        }
        default: {
            if (Data.FloatVector.Data != nullptr) {
                delete[] Data.FloatVector.Data;
                Uniform::S_Allocations--;
                Data.FloatVector.Data = nullptr;
            }
        }
        }
    }



}