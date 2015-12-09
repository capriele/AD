%% drone derivatives
function dx = droneStateDerv(t,x,u, quadverbose,quadd,quadh,quadr,quadtheta0,quadtheta1,quadgamma,quadCt,quadrho,quadA,quadCq,quadg,quadM,quadJ)
    global a1s b1s
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
    %CONSTANTS
    %Cardinal Direction Indicies
    N = 1;                      %   N       'North'                             1x1
    E = 2;                      %   S       'South'                             1x1
    S = 3;                      %   E       'East'                              1x1
    W = 4;                      %   W       'West'                              1x1
    
    
    d=sqrt(2)/2*quad.d;
    
    D(:,1) = [ d;-d;quad.h];    %   Di      Rotor hub displacements             1x3
    D(:,2) = [ d; d;quad.h];
    D(:,3) = [-d; d;quad.h];
    D(:,4) = [-d;-d;quad.h]; 
    
    %Body-fixed frame references
    e1 = [1;0;0];               %   ei      Body fixed frame references         3x1
    e2 = [0;1;0];
    e3 = [0;0;1];
    
    %EXTRACT STATES FROM U
    w = u(1:4);
    
    %EXTRACT STATES FROM X
    z = x(1:3);   % position in {W}
    n = x(4:6);   % RPY angles {W}
    v = x(7:9);   % velocity in {W}
    o = x(10:12); % angular velocity in {W} (body pqr? (F))
    
    %PREPROCESS ROTATION AND WRONSKIAN MATRICIES
    phi = n(1);    % yaw
    the = n(2);    % pitch
    psi = n(3);    % roll

    if (abs(the)>pi/2) error('|pitch| greater than pi/2!'); end;
    if (abs(psi)>pi/2) error('|roll| greater than pi/2!'); end;
    if (z(3)>0) error('z greater than 0 (under ground)!'); end;  

    
    % rotz(phi)*roty(the)*rotx(psi)
    R = [cos(the)*cos(phi) sin(psi)*sin(the)*cos(phi)-cos(psi)*sin(phi) cos(psi)*sin(the)*cos(phi)+sin(psi)*sin(phi);   %BBF > Inertial rotation matrix
         cos(the)*sin(phi) sin(psi)*sin(the)*sin(phi)+cos(psi)*cos(phi) cos(psi)*sin(the)*sin(phi)-sin(psi)*cos(phi);
         -sin(the)         sin(psi)*cos(the)                            cos(psi)*cos(the)];
    
    
    %Manual Construction
    %     Q3 = [cos(phi) -sin(phi) 0;sin(phi) cos(phi) 0;0 0 1];   % RZ %Rotation mappings
    %     Q2 = [cos(the) 0 sin(the);0 1 0;-sin(the) 0 cos(the)];   % RY
    %     Q1 = [1 0 0;0 cos(psi) -sin(psi);0 sin(psi) cos(psi)];   % RX
    %     R = Q3*Q2*Q1    %Rotation matrix
    %
    %    RZ * RY * RX
    iW = [0        sin(psi)          cos(psi);             %inverted Wronskian (body rates p-q-r to euler rates yaw pitch roll)
          0        cos(psi)*cos(the) -sin(psi)*cos(the);
          cos(the) sin(psi)*sin(the) cos(psi)*sin(the)] / cos(the);
    
    %ROTOR MODEL
    for i=[N E S W] %for each rotor
        %Relative motion
        
        Vr = cross(o,D(:,i)) + inv(R)*v;
        mu = sqrt(sum(Vr(1:2).^2)) / (abs(w(i))*quad.r);  %Magnitude of mu, planar components
        lc = Vr(3) / (abs(w(i))*quad.r);                  %Non-dimensionalised normal inflow
        li = mu;                                          %Non-dimensionalised induced velocity approximation
        alphas = atan2(lc,mu);
        j = atan2(Vr(2),Vr(1));                           %Sideslip azimuth relative to e1 (zero over nose)
        J = [cos(j) -sin(j);
            sin(j) cos(j)];                               %BBF > mu sideslip rotation matrix
        
        %Flapping
        beta = [((8/3*quad.theta0 + 2*quad.theta1)*mu - 2*(lc)*mu)/(1-mu^2/2); %Longitudinal flapping
            0;];                                          %sign(w) * (4/3)*((Ct/sigma)*(2*mu*gamma/3/a)/(1+3*e/2/r) + li)/(1+mu^2/2)]; %Lattitudinal flapping (note sign)
        beta = J'*beta;                                   %Rotate the beta flapping angles to longitudinal and lateral coordinates.
        a1s(i) = beta(1) - 16/quad.gamma/abs(w(i)) * o(2);
        b1s(i) = beta(2) - 16/quad.gamma/abs(w(i)) * o(1);
        
        %Forces and torques
        T(:,i) = quad.Ct*quad.rho*quad.A*quad.r^2*w(i)^2 * [-cos(b1s(i))*sin(a1s(i)); sin(b1s(i));-cos(a1s(i))*cos(b1s(i))];   %Rotor thrust, linearised angle approximations
        Q(:,i) = -quad.Cq*quad.rho*quad.A*quad.r^3*w(i)*abs(w(i)) * e3;     %Rotor drag torque - note that this preserves w(i) direction sign
        %tau(:,i) = cross(T(:,i),D(:,i));                %Torque due to rotor thrust
        tau(:,i) = cross(D(:,i),T(:,i));                 %changed sign (F)
    end
    
    %RIGID BODY DYNAMIC MODEL
    dz = v;
    dn = iW*o;
    
    dv = (quad.g*e3 + R*(1/quad.M)*sum(T,2));
    do = inv(quad.J)*(-cross(o,quad.J*o) + sum(tau,2) + sum(Q,2)); %row sum of torques
    dx = [dz;dn;dv;do];                                           %This is the state derivative vector
    
    
    
end % End of mdlDerivatives.




