%==============================================================
% mdlOutputs
% Calculate the output vector for this timestep
%==============================================================
%
function sys = droneOutputs(t,x, quadverbose,quadd,quadh,quadr,quadtheta0,quadtheta1,quadgamma,quadCt,quadrho,quadA,quadCq,quadg,quadM,quadJ)

%rebuilt quad.
quad.verbose=quadverbose;
quad.d=quadd;
quad.h=quadh;
quad.r=quadr;
quad.theta0=quadtheta0;
quad.theta1=quadtheta1;
quad.gamma=quadgamma;
quad.Ct=quadCt;
quad.rho=quadrho;
quad.A=quadA;
quad.Cq=quadCq;
quad.g=quadg;
quad.M=quadM;
quad.J=quadJ;

    %CRASH DETECTION
    if x(3)>0
        x(3) = 0;
        if x(6) > 0
            x(6) = 0;
        end
    end
    %if (x(3)>0)&(crash)
    %    error('CRASH!')
    %end
    
    %TELEMETRY
    if quad.verbose
        disp(sprintf('%0.3f\t',t,x))
    end
    
    % compute output vector as a function of state vector
    %   z      Position                         3x1   (x,y,z)
    %   v      Velocity                         3x1   (xd,yd,zd)
    %   n      Attitude                         3x1   (Y,P,R)
    %   o      Angular velocity                 3x1   (Yd,Pd,Rd)
    
    n = x(4:6);   % RPY angles
    phi = n(1);    % yaw
    the = n(2);    % pitch
    psi = n(3);    % roll
    
    
    % rotz(phi)*roty(the)*rotx(psi)
    R = [cos(the)*cos(phi) sin(psi)*sin(the)*cos(phi)-cos(psi)*sin(phi) cos(psi)*sin(the)*cos(phi)+sin(psi)*sin(phi);   %BBF > Inertial rotation matrix
         cos(the)*sin(phi) sin(psi)*sin(the)*sin(phi)+cos(psi)*cos(phi) cos(psi)*sin(the)*sin(phi)-sin(psi)*cos(phi);
         -sin(the)         sin(psi)*cos(the)                            cos(psi)*cos(the)];
    
    iW = [0        sin(psi)          cos(psi);             %inverted Wronskian
          0        cos(psi)*cos(the) -sin(psi)*cos(the);
          cos(the) sin(psi)*sin(the) cos(psi)*sin(the)] / cos(the);
    
    % return velocity in the body frame
    sys = [ x(1:6);             %output global pos and euler angles
            inv(R)*x(7:9);      %translational velocity mapped to body frame -> i.e. output v in bodyframe! (F)
            x(10:12)];%iW*x(10:12)];    % RPY rates mapped to body frame ->     i.e. : this outputed euler rates!, now in bodyrates pqr (F)
    %sys = [x(1:6); iW*x(7:9);  iW*x(10:12)];
    %sys = x;
end
% End of mdlOutputs.
