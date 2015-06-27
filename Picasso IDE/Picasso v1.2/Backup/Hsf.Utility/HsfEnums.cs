namespace Hsf.Utility
{
    public enum PositionType
    {
        StaticLLA,
        StaticECI,
        PredeterminedLLA,
        PredeterminedECI,
        DynamicLLA,
        DynamicECI
    }

    public enum TargetType
    {
        LocationTarget,
        FacilityTarget,
        PersonnelTarget,
        GroundVehicleTarget,
        AirVehicleTarget,
        BallisticTarget,
        SpaceVehicleTarget,
        UnknownTarget
    }

    public enum TaskType
    {
        ImagingTask,
        CommTask,
        SelfTask,
        RendezvousTask,
        DestroyTask,
        NoTask
    }

    public enum ScheduleEvaluatorType
    {
        TargetValueScheduleEvaluator,
        QuantitativeScheduleEvaluator,
        scripted
    }

    public enum EomsType
    {
        Scripted,
        Orbital_EOMS,
        None
    }

    public enum SubsystemType
    {
        Scripted,
        Access,
        Adcs,
        EOSensor,
        Ssdr,
        Comm,
        Power
    }

    public enum HsfType
    {
        Double,
        Bool,
        Matrix,
        Float,
        Quat,
        Int,
        String,
        None
    }

    public enum ConstraintType
    {
        FAIL_IF_HIGHER,
        FAIL_IF_LOWER,
        IGNORE
    }
}