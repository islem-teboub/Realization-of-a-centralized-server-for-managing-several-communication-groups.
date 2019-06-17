

package part2;

simple Client
{
    @display("i=device/pc");
    gates:
        inout inoutport;
}

simple Server
{
    @display("i=device/server");
    gates:
        inout inoutport[];


}
network Part2
{
    parameters:
        int x ;
        int i = 0;

    submodules:
        serveur: Server;
        client[x]: Client;
    connections:

        for i=0..x-1 {
            client[i].inoutport <--> {  delay = 500 ms; } <--> serveur.inoutport++;

        }

}

