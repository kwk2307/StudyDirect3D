#pragma once
#define MAX_SPEED 300

class Player :
    public Object
{
public:
    Player(const ObjectInfo& Info);

public:
    void Update(float InDeltaSeconds) override;

    virtual void OnCollision(const Collider& InOther) override;

    virtual void BeginCollision(const Collider& InOther) override;

    virtual void EndCollision(const Collider& InOther) override;

private:
    Vector3 _Speed = Vector3::Zero;
    bool isLand = false;
};

